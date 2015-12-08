#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>
#include "shell_platform.h"


#ifndef SHELL_WELCOM_MESSAGE
#define SHELL_WELCOM_MESSAGE     "Hello, this is a shell."
#endif

#ifndef SHELL_PROMPT
#define SHELL_PROMPT              "SHELL> "
#endif

#define SHELL_MAXSIZE             50
#define SHELL_ERRMSG              "Invalid command, type 'help' for help\n"
#define SHELL_ALT_SPACE           '\x07'
#define SHELL_MAX_ARGS            10

static void shellh_not_implemented_handler(int argc, char **argv) {
    (void)argc;
    (void)argv;
    printf(SHELL_ERRMSG);
}

static void shellh_show_help(const char *cmd, const char *helptext) {
    printf("Usage: %s %s", cmd, helptext);
}

static const char shell_summary_exit[] = "exit the shell";
static const char shell_help_exit[] = "\nExits the shell.\n";
static void shell_func_exit(int argc, char **argv) {
    (void)argc;
    (void)argv;
    printf("byte ...\n");
}

static const char shell_summary_help[] = "shell help";
static const char shell_help_help[] = "[<command>]\n"
                                      "    [<command>] - the command to get help on.\n"
                                      "Without arguments it shows a summary of all the shell commands.\n";
static void shell_func_help(int argc, char **argv);


/// shell命令列表的结束标记.
#define SHELL_COMMAND_END() {(const char *__FAR)0, (const char *__FAR)0, (const char *__FAR)0, (const char *__FAR)0}

static const struct shell_command buildin_shell_commands[] = {
#ifdef SHELL_COMMAND_CUSTOM_LIST
    SHELL_COMMAND_CUSTOM_LIST
#endif
    SHELL_COMMAND("help", help),
    SHELL_COMMAND("exit", exit),
    SHELL_COMMAND_END(),
};

static void shell_list_summary_help(const struct shell_command *cmds) {
    const struct shell_command *__FAR pcmd;
    for (pcmd = cmds; pcmd->cmd != NULL; ++pcmd) {
        if (strlen(pcmd->summary) > 0) {
            printf("  %-6s - %s\n", pcmd->cmd, pcmd->summary);
        }
    }
}

static const struct shell_command *shell_detail_help(const struct shell_command *cmds, const char *cmd) {
    const struct shell_command *pcmd;
    for (pcmd = cmds; pcmd->cmd != NULL; ++pcmd) {
        if (!strcmp(pcmd->cmd, cmd) && strlen(pcmd->summary) > 0) {
            printf("%s - %s\nUsage:\n %s %s\n\n",
                   pcmd->cmd,
                   pcmd->summary,
                   pcmd->cmd,
                   pcmd->help);
            return pcmd;
        }
    }
    return NULL;
}


static void shell_func_help(int argc, char **argv) {
    if (argc > 2) {
        shellh_show_help(argv[0], shell_help_help);
        return;
    }

    if (argc == 1) {
        printf("Shell commands:\n");
        shell_list_summary_help(buildin_shell_commands);
        return;
    }

    if (NULL != shell_detail_help(buildin_shell_commands, argv[1])) {
        return;
    }

    printf("Unknown command '%s'.\n", argv[ 1 ]);
    printf("For more information use 'help <command>'.\n");
}

static const struct shell_command *shell_execute_command_in_commands(const struct shell_command *cmds, int argc, char **argv) {
    const struct shell_command *pcmd;
    for (pcmd = cmds; pcmd->cmd != NULL; ++pcmd) {
        if (!strcmp(pcmd->cmd, argv[0])) {
            pcmd->handler(argc, argv);
            return pcmd;
        }
    }
    return NULL;
}

void shell_execute_command(char *cmd) {
    char *p, *temp;
    int i, inside_quotes;
    char quote_char;
    int argc;
    char *argv[ SHELL_MAX_ARGS ];

    if (strlen(cmd) == 0) {
        return;
    }

    if (cmd[ strlen(cmd) - 1 ] != '\n') {
        strcat(cmd, "\n");
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
        printf("Invalid quoted string\n");
        return;
    }

    // Transform consecutive sequences of spaces into a single space
    p = strchr(cmd, ' ');
    while (p) {
        temp = p + 1;
        while (*temp && *temp == ' ') {
            memmove(temp, temp + 1, strlen(temp));
        }
        p = strchr(p + 1, ' ');
    }
    if (!strcmp(cmd, " ")) {
        return;
    }

    // Skip over the initial space char if it exists
    p = cmd;
    if (*p == ' ') {
        p ++;
    }

    // Add a final space if it does not exist
    if (p[ strlen(p) - 1 ] != ' ') {
        strcat(p, " ");
    }

    // Compute argc/argv
    for (argc = 0; argc < SHELL_MAX_ARGS; argc ++) {
        argv[ argc ] = NULL;
    }
    argc = 0;
    while ((temp = strchr(p, ' ')) != NULL) {
        *temp = 0;
        if (argc == SHELL_MAX_ARGS) {
            printf("Error: too many arguments\n");
            argc = -1;
            break;
        }
        argv[ argc ++ ] = p;
        p = temp + 1;
    }

    if (argc == -1) {
        return;
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

    if (NULL != shell_execute_command_in_commands(buildin_shell_commands, argc, argv)) {
        return;
    }


    printf("Command \"%s\" cannot be found.\n", argv[0]);
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
                putchar('\b');
                putchar(' ');
                putchar('\b');
            }
            continue;
        }

        if (c == '\r') {
            putchar('\n');
            *buf = 0;
            return;
        }

        putchar(c);
        *buf++ = (char)c;
        ++i;
    }

    *buf = 0;
}

void shell_loop(void) {
    char cmd[SHELL_MAXSIZE];


    printf("\n");
    printf(SHELL_WELCOM_MESSAGE);
    printf("\n");

    while (1) {
        printf(SHELL_PROMPT);
        getline(cmd, sizeof(cmd));
        if (strlen(cmd) == 0) {
            continue;
        }
        shell_execute_command(cmd);
    }
}

