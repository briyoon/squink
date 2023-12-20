#include "squink.h"
#include <stdio.h>

int main(int argc, char *argv[]) {
  FILE *fp = fopen("test.log", "w");
  log_add_file_sink(fp, LEVEL_DEBUG);
  log_add_console_sink(stdout, LEVEL_DEBUG);

  for (int i = 0; i < 1000000; i++)
    log_debug("loop: %d", i);

  log_free_sinks();
}