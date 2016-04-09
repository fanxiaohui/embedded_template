#ifndef __SHELL_PLATFORM_H__
#define __SHELL_PLATFORM_H__

#include "shell.h"

#ifndef __FAR
#define __FAR
#endif

/// shell��Ӧ��ִ�к���.
typedef int(*shell_handler)(int argc, char **argv);

#ifndef SHELL_SUPPORT_HELP
#define SHELL_SUPPORT_HELP 1
#endif

/// һ��shell����������Ϣ.
struct shell_command {
    const char *cmd; ///< shell����.
#if SHELL_SUPPORT_HELP
    const char *summary; ///< ��Ҫ����.
    const char *help; ///< ��ϸ����
#endif
    shell_handler handler; ///< ��Ӧִ�еĺ���.
};


/// ���ڳ�ʼ��һ��shell����Ľṹ��.
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
