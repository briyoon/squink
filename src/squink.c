#include "squink.h"
#include "sinks/base.h"
#include "sinks/console.h"
#include "sinks/file.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>

struct log_settings LOG_SETTINGS;
char *level_strings[] = {"DEBUG", "INFO", "WARNING", "ERROR", "FATAL"};
char *level_colors[] = {ANSI_COLOR_CYAN, ANSI_COLOR_GREEN, ANSI_COLOR_YELLOW,
                        ANSI_COLOR_RED, ANSI_COLOR_MAGENTA};

void log_add_file_sink(FILE *fp, enum log_level level) {
  struct log_sink_file *sink = calloc(1, sizeof(struct log_sink_file));

  sink->base.enabled = true;
  sink->base.level = level;
  sink->base.stream = fp;
  sink->base.buffer_size = 1024;
  sink->base.flush_latency_ms = 250;
  sink->base.last_flush_ms = 0;
  sink->base.buffer = calloc(sink->base.buffer_size, sizeof(char *));
  sink->base.buffer_index = 0;

  for (int i = 0; i < sink->base.buffer_size; i++) {
    sink->base.buffer[i] = calloc(MAX_MSG_SIZE, sizeof(char));
  }

  struct log_sink_node *head = calloc(1, sizeof(struct log_sink_node));

  head->type = SINK_FILE;
  head->sink = (struct log_sink_base *)sink;

  if (LOG_SETTINGS.sinks != NULL) {
    head->next = LOG_SETTINGS.sinks;
  }

  LOG_SETTINGS.sinks = head;
}

void log_add_console_sink(FILE *std, enum log_level level) {
  if (!isatty(fileno(std))) {
    return;
  }

  struct log_sink_console *sink = calloc(1, sizeof(struct log_sink_console));

  sink->base.enabled = true;
  sink->base.level = level;
  sink->base.stream = std;
  sink->base.buffer_size = 1024;
  sink->base.flush_latency_ms = 250;
  sink->base.last_flush_ms = 0;
  sink->base.buffer = calloc(sink->base.buffer_size, sizeof(char *));
  sink->base.buffer_index = 0;

  for (int i = 0; i < sink->base.buffer_size; i++) {
    sink->base.buffer[i] = calloc(MAX_MSG_SIZE, sizeof(char));
  }
  sink->color = true;

  struct log_sink_node *head = calloc(1, sizeof(struct log_sink_node));

  head->type = SINK_CONSOLE;
  head->sink = (struct log_sink_base *)sink;

  if (LOG_SETTINGS.sinks != NULL) {
    head->next = LOG_SETTINGS.sinks;
  }

  LOG_SETTINGS.sinks = head;
}

void log_free_sinks() {
  struct log_sink_node *current_sink = LOG_SETTINGS.sinks;

  while (current_sink != NULL) {
    struct log_sink_node *next = current_sink->next;

    struct log_sink_base *sink = current_sink->sink;

    for (int i = 0; i < sink->buffer_size; i++) {
      free(sink->buffer[i]);
    }

    free(sink->buffer);

    free(current_sink->sink);
    free(current_sink);
    current_sink = next;
  }
}

void log_log(enum log_level level, char *file, int line, const char *fmt, ...) {
  struct log_sink_node *current_sink = LOG_SETTINGS.sinks;

  bool init_flag = false;
  struct timeval tv;
  size_t tv_ms;
  char time_str[24];     // = "";
  const char *level_str; // = "";
  char message[512];     // = "";

  while (current_sink != NULL) {
    // check if we can slip this sink
    struct log_sink_base *base = (struct log_sink_base *)current_sink->sink;
    bool dont_log = !base->enabled || level < base->level;
    if (dont_log) {
      current_sink = current_sink->next;
      continue;
    }

    // if this is the first writeable sink, we need to init variables
    if (!init_flag) {
      init_flag = true;
      // time
      gettimeofday(&tv, NULL);
      time_t now = tv.tv_sec;
      tv_ms = tv.tv_usec / 1000;
      strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S",
               localtime(&now));
      snprintf(time_str + 19, 5, ".%03lu", tv_ms);

      // level
      level_str = level_strings[level];

      // message
      va_list args;
      va_start(args, fmt);
      vsnprintf(message, sizeof(message), fmt, args);
    }

    // add to buffer
    bool print_color = current_sink->type == SINK_CONSOLE &&
                       ((struct log_sink_console *)base)->color;
    if (print_color) {
      snprintf(message, sizeof(message), "[%s %s%s%s] %s%s:%d%s %s\n", time_str,
               level_colors[level], level_str, ANSI_COLOR_RESET,
               ANSI_COLOR_BLACK, file, line, ANSI_COLOR_RESET, message);
    } else {
      snprintf(message, sizeof(message), "[%s %s] %s:%d %s\n", time_str,
               level_str, file, line, message);
    }
    log_add_to_buffer(current_sink->sink, message);

    // flush buffer if enough time has passed
    bool flush_threshold = base->last_flush_ms - ((tv.tv_sec * 1000) + tv_ms) <
                           base->flush_latency_ms;
    if (flush_threshold) {
      log_flush_buffer(current_sink->sink);
    }

    current_sink = current_sink->next;
  }
}