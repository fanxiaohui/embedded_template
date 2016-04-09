#ifndef __SHELL_PLATFORM_H__
#define __SHELL_PLATFORM_H__

#include "shell.h"

#ifndef __FAR
#define __FAR
#endif

/// shell对应的执行函数.
typedef int(*shell_handler)(int argc, char **argv);

#ifndef SHELL_SUPPORT_HELP
#define SHELL_SUPPORT_HELP 1
#endif

/// 一个shell命令的相关信息.
struct shell_command {
    const char *cmd; ///< shell命令.
#if SHELL_SUPPORT_HELP
    const char *summary; ///< 简要帮助.
    const char *help; ///< 详细帮助
#endif
    shell_handler handler; ///< 对应执行的函数.
};


/// 用于初始化一个shell命令的结构体.
#if SHELL_SUPPORT_HELP

#define SHELL_PROTYPE(cmd)  extern int shell_func_##cmd(int argc, char **argv); extern const char shell_help_##cmd[]; extern const char shell_summary_##cmd[]
SHELL_PROTYPE(help);

#define SHELL_COMMAND(cmd, shell) {cmd, shell_summary_##shell, shell_help_##shell, shell_func_##shell }
#define SHELL_COMMAND_END() {(const char *)0, (const char *)0, (const char *)0, (shell_handler)0}

#else // SHELL_SUPPORT_HELP

#define SHELL_PROTYPE(cmd) extern int shell_func_##cmd(int argc, char **argv); 

#define SHELL_COMMAND(cmd, shell) {cmd, shell_func_##shell }
#define SHELL_COMMAND_END() {(const char *)0, (shell_handler)0 }

#endif // SHELL_SUPPORT_HELP

    
#endif
