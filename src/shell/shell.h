#ifndef __SHELL_H__
#define __SHELL_H__

typedef void (*shell_get_line)(char *buf, int buffer_size, void *priv_data);

/// \brief shell_loop ִ��shellѭ��.
void shell_loop(shell_get_line, void *priv_data);

/// \brief shell_ask_yes_no ��stdinѯ��, ��ѡ��Yes or No.
///
/// \param prompt ѯ��ʱ�����ʾ��.
///
/// \return !=0 �������Yes; ==0 �������No.
//char shell_ask_yes_no(const char *prompt);

#endif

