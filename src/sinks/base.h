#ifndef SQUINK_SINKS_BASE_H
#define SQUINK_SINKS_BASE_H

#include "squink.h"
#include <stdbool.h>
#include <stddef.h>

struct log_sink_base {
  bool enabled;
  enum log_level level;
  size_t flush_latency_ms;
  size_t time_since_flush_ms;
  size_t buffer_size;
};

#endif // SQUINK_SINKS_BASE_H