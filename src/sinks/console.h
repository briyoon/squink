#ifndef LOG_CONSOLE_SINK_H
#define LOG_CONSOLE_SINK_H

#include "base.h"

#define ANSI_COLOR_BLACK "\x1b[30m"
#define ANSI_COLOR_RED "\x1b[31m"
#define ANSI_COLOR_GREEN "\x1b[32m"
#define ANSI_COLOR_YELLOW "\x1b[33m"
#define ANSI_COLOR_BLUE "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN "\x1b[36m"
#define ANSI_COLOR_WHITE "\x1b[37m"
#define ANSI_COLOR_RESET "\x1b[0m"

struct log_sink_console {
  struct log_sink_base base;
  FILE *stream;
  bool color;
};

void log_log_console(struct log_sink_console *sink, enum log_level level,
                     char *file, int line, char *msg);

#endif // LOG_CONSOLE_SINK_H