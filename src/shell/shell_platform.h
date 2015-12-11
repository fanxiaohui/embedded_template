#ifndef __SHELL_PLATFORM_H__
#define __SHELL_PLATFORM_H__

#include "shell.h"

/// shell对应的执行函数.
typedef int(*shell_handler)(int argc, char **argv);

/// 一个shell命令的相关信息.
struct shell_command {
    const char *cmd; ///< shell命令.
    const char *summary; ///< 简要帮助.
    const char *help; ///< 详细帮助
    shell_handler handler; ///< 对应执行的函数.
};

/// 用于声明一个shell命令.
#define SHELL_PROTYPE(cmd)  \
    extern int shell_func_##cmd(int argc, char **argv); \
    extern const char shell_help_##cmd[]; \
    extern const char shell_summary_##cmd[]

/// 用于初始化一个shell命令的结构体.
#define SHELL_COMMAND(cmd, shell) \
    {cmd, shell_summary_##shell, shell_help_##shell, shell_func_##shell }

#include "shell_config.h"

#endif

