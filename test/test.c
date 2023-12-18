#include "../include/squink.h"
#include <stdio.h>

int main(int argc, char *argv[]) {
  FILE *fp = fopen("log.txt", "w");

  log_add_fp(fp, LEVEL_DEBUG);

  for (int i = 0; i < 1000000; i++)
    // fprintf(fp, "loop: %d\n", i);
    log_debug("loop: %d", i);

  fclose(fp);
}