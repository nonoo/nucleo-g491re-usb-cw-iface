#ifndef LOG_H_
#define LOG_H_

#include "uart-console.h"

#if LOGGING_ENABLED
#define LOG(TAG, format, ...) uart_console_log(TAG, format, ##__VA_ARGS__)
#define LOGC(format, ...) uart_console_log(NULL, format, ##__VA_ARGS__)
#define DBG(TAG, format, ...) uart_console_log(TAG, format, ##__VA_ARGS__)
#define DBGC(format, ...) uart_console_log(NULL, format, ##__VA_ARGS__)
#else
#define LOG(TAG, format, ...) while (0) {}
#define LOGC(format, ...) while (0) {}
#define DBG(TAG, format, ...) while (0) {}
#define DBGC(format, ...) while (0) {}
#endif

#endif // LOG_H_
