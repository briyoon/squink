#ifndef SQUINK_H
#define SQUINK_H

#include <stdio.h>

enum log_level {
  LEVEL_DEBUG,
  LEVEL_INFO,
  LEVEL_WARNING,
  LEVEL_ERROR,
  LEVEL_FATAL
};

enum log_template {
  TEMPLATE_TIME,
  TEMPLATE_LEVEL,
  TEMPLATE_FILE,
  TEMPLATE_LINE,
  TEMPLATE_MESSAGE
};

enum log_sink_type { SINK_CONSOLE, SINK_FILE };

struct log_sink_node {
  enum log_sink_type type;
  void *sink;
  struct log_sink_node *next;
};

struct log_settings {
  struct log_sink_node *sinks;
};

const char *log_get_level(enum log_level level);

void log_add_file_sink(FILE *fp, enum log_level);
void log_add_console_sink(FILE *std, enum log_level);
void log_free_sinks();

void log_log(enum log_level level, char *file, int line, const char *fmt, ...);

#define log_debug(fmt, ...)                                                    \
  log_log(LEVEL_DEBUG, __FILE__, __LINE__, fmt, __VA_ARGS__)
#define log_info(fmt, ...)                                                     \
  log_log(LEVEL_INFO, __FILE__, __LINE__, fmt, __VA_ARGS__)
#define log_warning(fmt, ...)                                                  \
  log_log(LEVEL_WARNING, __FILE__, __LINE__, fmt, __VA_ARGS__)
#define log_error(fmt, ...)                                                    \
  log_log(LEVEL_ERROR, __FILE__, __LINE__, fmt, __VA_ARGS__)
#define log_fatal(fmt, ...)                                                    \
  log_log(LEVEL_FATAL, __FILE__, __LINE__, fmt, __VA_ARGS__)

#endif // SQUINK_H