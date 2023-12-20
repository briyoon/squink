#include "squink.h"
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
                        ANSI_COLOR_RED, ANSI_COLOR_BLACK};

void log_add_file_sink(FILE *fp, enum log_level level) {
  struct log_sink_file *sink =
      (struct log_sink_file *)malloc(sizeof(struct log_sink_file));

  sink->base.enabled = true;
  sink->base.level = level;
  sink->fp = fp;

  struct log_sink_node *head =
      (struct log_sink_node *)malloc(sizeof(struct log_sink_node));

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

  struct log_sink_console *sink =
      (struct log_sink_console *)malloc(sizeof(struct log_sink_console));

  sink->base.enabled = true;
  sink->base.level = level;
  sink->stream = std;
  sink->color = true;

  struct log_sink_node *head =
      (struct log_sink_node *)malloc(sizeof(struct log_sink_node));

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
    free(current_sink->sink);
    free(current_sink);
    current_sink = next;
  }
}

void log_log(enum log_level level, char *file, int line, const char *fmt, ...) {
  struct log_sink_node *current_sink = LOG_SETTINGS.sinks;

  bool init_flag = false;
  char time_str[24];
  const char *level_str;
  char message[512];

  while (current_sink != NULL) {
    struct log_sink_base *base = (struct log_sink_base *)current_sink->sink;
    if (!base->enabled || level < base->level) {
      break;
    }

    if (init_flag == false) {
      // time
      struct timeval tv;
      time_t now = tv.tv_sec;
      gettimeofday(&tv, NULL);
      strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S",
               localtime(&now));
      snprintf(time_str + 19, 5, ".%03d", (int)(tv.tv_usec / 1000));

      // level
      level_str = level_strings[level];

      // message
      va_list args;
      va_start(args, fmt);
      vsnprintf(message, sizeof(message), fmt, args);

      init_flag = true;
    }

    switch (current_sink->type) {
    case SINK_FILE:
      fprintf(((struct log_sink_file *)current_sink->sink)->fp,
              "[%s %s] %s:%d %s\n", time_str, level_str, file, line, message);
      break;
    case SINK_CONSOLE:
      if (((struct log_sink_console *)current_sink->sink)->color) {
        fprintf(((struct log_sink_console *)current_sink->sink)->stream,
                "[%s %s%s%s] %s:%d %s\n", time_str, level_colors[level],
                level_str, ANSI_COLOR_RESET, file, line, message);
      } else {
        fprintf(((struct log_sink_console *)current_sink->sink)->stream,
                "[%s %s] %s:%d %s\n", time_str, level_str, file, line, message);
      }
      break;
    default:
      break;
    }

    current_sink = current_sink->next;
  }
}
