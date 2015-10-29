#ifndef __SHELL_PLATFORM_H__
#define __SHELL_PLATFORM_H__

//#define SHELL_WELCOM_MESSAGE "Hello, this is a shell."
//#define SHELL_PROMPT         "eLua# "
SHELL_PROTYPE(led);
SHELL_PROTYPE(echo);

#define SHELL_COMMAND_CUSTOM_LIST \
    SHELL_COMMAND("echo", echo), \
    SHELL_COMMAND("led", led)

#endif

