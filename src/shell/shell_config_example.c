#include "shell_platform.h"


const char *const shell_wellcome_message = "Hello, this is a shell.";
const char *const shell_prompt = "Shell> ";
const struct shell_command shell_commands[] = {
    SHELL_COMMAND("led", led),
    SHELL_COMMAND_END(),
};