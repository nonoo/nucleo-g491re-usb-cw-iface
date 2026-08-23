#ifndef UART_CONSOLE_H_
#define UART_CONSOLE_H_

#if UART_CONSOLE_ENABLED

void uart_console_log(const char *tag, const char *fmt_s, ...);
void uart_console_init(void);

#endif

#endif // UART_CONSOLE_H_
