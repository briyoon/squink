#ifndef LOG_CONSOLE_SINK_H
#define LOG_CONSOLE_SINK_H

#include "base.h"

enum log_output_stream { STREAM_STDOUT, STREAM_STDERR };

struct log_sink_console {
  struct log_sink_base base;
  FILE *stream;
  bool color;
};

void log_log_console(struct log_sink_console *sink, enum log_level level,
                     char *file, int line, char *msg);

#endif // LOG_CONSOLE_SINK_H