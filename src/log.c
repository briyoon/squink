#include "log.h"
#include <stdarg.h>
#include <stdio.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>

struct log_settings LOG_SETTINGS;

void log_init(enum log_level level) { log_set_level(level); }

void log_set_level(enum log_level level) { LOG_SETTINGS.level = level; }

const char *log_get_level(enum log_level level) {
  switch (level) {
  case LEVEL_DEBUG:
    return "DEBUG";
  case LEVEL_INFO:
    return "INFO";
  case LEVEL_WARNING:
    return "WARNING";
  case LEVEL_ERROR:
    return "ERROR";
  case LEVEL_FATAL:
    return "FATAL";
  default:
    return "UNKNOWN";
  }
}

void log_log(enum log_level level, char *file, int line, const char *fmt, ...) {
  if (level < LOG_SETTINGS.level) {
    return;
  }

  // time
  struct timeval tv;
  gettimeofday(&tv, NULL);
  time_t now = tv.tv_sec;
  char time_str[24];
  strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", localtime(&now));
  snprintf(time_str + 19, 5, ".%03d", (int)(tv.tv_usec / 1000));

  // level
  const char *level_str = log_get_level(level);

  // message
  char message[512];
  va_list args;
  va_start(args, fmt);
  vsnprintf(message, sizeof(message), fmt, args);

  fprintf(LOG_SETTINGS.fp, "[%s %s] %s:%d: %s\n", time_str, level_str, file,
          line, message);
}

void log_add_fp(FILE *fp, enum log_level level) {
  LOG_SETTINGS.fp = fp;
  LOG_SETTINGS.level = level;
}
