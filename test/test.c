#include "squink.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
  FILE *fp = fopen("test.log", "w");
  log_add_file_sink(fp, LEVEL_DEBUG);
  log_add_console_sink(stdout, LEVEL_DEBUG);

  for (int i = 0; i < 10000; i++) {
    int random = rand() % 5;

    switch (random) {
    case 0:
      log_debug("loop: %d", i);
      break;
    case 1:
      log_info("loop: %d", i);
      break;
    case 2:
      log_warning("loop: %d", i);
      break;
    case 3:
      log_error("loop: %d", i);
      break;
    case 4:
      log_fatal("loop: %d", i);
      break;
    }
  }

  log_free_sinks();
  fclose(fp);
}