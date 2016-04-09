#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>

#include "shell_platform.h"

extern const char *const shell_wellcome_message;
extern const char *const shell_prompt;
extern const struct shell_command shell_commands[];

#define SHELL_MAXSIZE             50
#define SHELL_ALT_SPACE           '\x07'
#define SHELL_MAX_ARGS            10


#if SHELL_SUPPORT_HELP

const char shell_summary_help[] = "shell help";
const char shell_help_help[] = "\
[<command>]\n\
    [<command>] - the command to get help on.\n\
Without arguments it shows a summary of all the shell commands.\n\
";

static void shell_list_summary_help(const struct shell_command *cmds) {
    const struct shell_command *__FAR pcmd;
    for (pcmd = cmds; pcmd->cmd != NULL; ++pcmd) {
        if (strlen(pcmd->summary) > 0) {
            (void)printf("  %-6s - %s\n", pcmd->cmd, pcmd->summary);
        }
    }
    (void)printf("\nhelp <command> for more information of the command.\n\n");
}

static const struct shell_command *shell_detail_help(const struct shell_command *cmds, const char *cmd) {
    const struct shell_command *pcmd;
    for (pcmd = cmds; pcmd->cmd != NULL; ++pcmd) {
        if (!strcmp(pcmd->cmd, cmd) && strlen(pcmd->summary) > 0) {
            (void)printf("%s - %s\nUsage:\n %s %s\n\n",
                         pcmd->cmd,
                         pcmd->summary,
                         pcmd->cmd,
                         pcmd->help);
            return pcmd;
        }
    }
    return NULL;
}

int shell_func_help(int argc, char **argv) {
    if (argc > 2) {
        (void)printf("Usage: %s %s", argv[0], shell_help_help);
        return -1;
    }

    if (argc == 1) {
        (void)printf("Shell commands:\n");
        shell_list_summary_help(shell_commands);
        return 0;
    }

    if (NULL != shell_detail_help(shell_commands, argv[1])) {
        return 0;
    }

    (void)printf("Unknown command '%s'.\n", argv[ 1 ]);
    (void)printf("For more information use 'help <command>'.\n");
    return -2;
}

#endif

static char shell_execute_command_in_commands(const struct shell_command *cmds, int argc, char **argv) {
    const struct shell_command *pcmd;
    for (pcmd = cmds; pcmd->cmd != NULL; ++pcmd) {
        if (!strcmp(pcmd->cmd, argv[0])) {
            return (char)pcmd->handler(argc, argv);
        }
    }


    (void)printf("Command \"%s\" cannot be found.\n", argv[0]);
    return -128;
}

char shell_execute_command(char *cmd, int rc) {
    char *p, *temp;
    int i, inside_quotes;
    char quote_char;
    int argc;
    char *argv[ SHELL_MAX_ARGS ];

    if (strlen(cmd) == 0) {
        return rc;
    }

    if (cmd[ strlen(cmd) - 1 ] != '\n') {
        (void)strcat(cmd, "\n");
    }

    // Change '\r', '\n' and '\t' chars to ' ' to ease processing
    p = cmd;
    while (*p) {
        if (*p == '\r' || *p == '\n' || *p == '\t') {
            *p = ' ';
        }
        p ++;
    }

    // Transform ' ' characters inside a '' or "" quoted string in
    // a 'special' char. We do this to let the user execute something
    // like "lua -e 'quoted string'" without disturbing the quoted
    // string in any way.
    for (i = 0, inside_quotes = 0, quote_char = '\0'; i < strlen(cmd); i ++)
        if ((cmd[ i ] == '\'') || (cmd[ i ] == '"')) {
            if (!inside_quotes) {
                inside_quotes = 1;
                quote_char = cmd[ i ];
            } else {
                if (cmd[ i ] == quote_char) {
                    inside_quotes = 0;
                    quote_char = '\0';
                }
            }
        } else if ((cmd[ i ] == ' ') && inside_quotes) {
            cmd[ i ] = SHELL_ALT_SPACE;
        }
    if (inside_quotes) {
        //(void)printf("Invalid quoted string\n");
        return -126;
    }

    // Transform consecutive sequences of spaces into a single space
    p = strchr(cmd, ' ');
    while (p) {
        temp = p + 1;
        while (*temp && *temp == ' ') {
            (void)memmove(temp, temp + 1, strlen(temp));
        }
        p = strchr(p + 1, ' ');
    }
    if (!strcmp(cmd, " ")) {
        return rc;
    }

    // Skip over the initial space char if it exists
    p = cmd;
    if (*p == ' ') {
        p ++;
    }

    // Add a final space if it does not exist
    if (p[ strlen(p) - 1 ] != ' ') {
        (void)strcat(p, " ");
    }

    // Compute argc/argv
    for (argc = 0; argc < SHELL_MAX_ARGS; argc ++) {
        argv[ argc ] = NULL;
    }
    argc = 0;
    while (1) {
        temp = strchr(p, ' ');
        if (temp == NULL) {
            break;
        }
        *temp = 0;
        if (argc == SHELL_MAX_ARGS) {
            //(void)printf("Error: too many arguments\n");
            argc = -1;
            break;
        }
        argv[ argc ++ ] = p;
        p = temp + 1;
    }

    if (argc == -1) {
        return -124;
    }

    // Additional argument processing happens here
    for (i = 0; i < argc; i ++) {
        p = argv[ i ];
        // Put back spaces if needed
        for (inside_quotes = 0; inside_quotes < strlen(argv[ i ]); inside_quotes ++) {
            if (p[ inside_quotes ] == SHELL_ALT_SPACE) {
                argv[ i ][ inside_quotes ] = ' ';
            }
        }
        // Remove quotes
        if ((p[ 0 ] == '\'' || p [ 0 ] == '"') && (p[ 0 ] == p[ strlen(p) - 1 ])) {
            argv[ i ] = p + 1;
            p[ strlen(p) - 1 ] = '\0';
        }
    }

    return shell_execute_command_in_commands(shell_commands, argc, argv);
}


static void getline(char *buf, int buf_size) {
    signed int i;
    int c;
    for (i = 0; i < buf_size - 1;) {
        c = getchar();
        if (c == '\b' || c == 0x7F) {
            if (i > 0) {
                --i;
                --buf;
            }
            continue;
        }

        if (c == '\r' || c == '\n') {
            *buf = 0;
            if (i > 0) {
                return;
            }
        }
        *buf++ = (char)c;
        ++i;
    }

    *buf = 0;
}

void shell_loop(void) {
    int rc = 0;
    char cmd[SHELL_MAXSIZE] = {0};

    (void)printf("\n");
    (void)printf(shell_wellcome_message);
    (void)printf("\n");

    for (;;) {
        (void)printf("[%d]%s# ", rc, shell_prompt);
        fflush(stdout);
        getline(cmd, sizeof(cmd) - 1);
        rc = shell_execute_command(cmd, rc);
    }
}

