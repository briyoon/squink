#include "base.h"

void log_add_to_buffer(void *sink, char *msg) {}

void log_flush_buffer(void *sink_ptr) {
  struct log_sink_base *sink = (struct log_sink_base *)sink_ptr;

  // loop thru and write out
  for (int i = 0; i < sink->buffer_size || sink->buffer[i][0] != '\0'; i++) {

    sink->buffer[i] = '\n';
  }
}