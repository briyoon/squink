#ifndef SQUINK_SINKS_BASE_H
#define SQUINK_SINKS_BASE_H

#include "squink.h"
#include <stdbool.h>
#include <stddef.h>

struct log_sink_base {
  bool enabled;
  enum log_level level;
  FILE *stream;
  size_t flush_latency_ms;
  size_t last_flush_ms;
  size_t buffer_size;
  char *buffer;
  size_t buffer_index;
};

void log_add_to_buffer(void *sink, char *msg);
void log_flush_buffer(void *sink);

#endif // SQUINK_SINKS_BASE_H