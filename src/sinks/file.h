#ifndef SQUINK_SINKS_FILE_H
#define SQUINK_SINKS_FILE_H

#include "base.h"
#include <stdio.h>

struct log_sink_file {
  struct log_sink_base base;
  FILE *fp;
};

void log_log_file(struct log_sink_file *sink, enum log_level level, char *file,
                  int line, char *msg);

#endif // SQUINK_SINKS_FILE_H