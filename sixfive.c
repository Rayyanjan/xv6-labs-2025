// user/sixfive.c

#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define BUF_SIZE 512

char *separators = " -\r\t\n./,";

int
is_separator(char c) {
  return strchr(separators, c) != 0;
}

/* process one open file descriptor and print numbers divisible by 5 or 6 */
static void
process_fd(int fd) {
  char buf[BUF_SIZE];
  int n;
  char number[16];
  int num_idx = 0;

  while ((n = read(fd, buf, sizeof(buf))) > 0) {
    for (int i = 0; i < n; i++) {
      char c = buf[i];
      if (c >= '0' && c <= '9') {
        if (num_idx < (int)sizeof(number) - 1) {
          number[num_idx++] = c;
        }
      } else {
        if (num_idx > 0) {
          number[num_idx] = 0;
          int val = atoi(number);
          if (val % 5 == 0 || val % 6 == 0) {
            printf("%d\n", val);
          }
          num_idx = 0;
        }
      }
    }
  }

  if (num_idx > 0) {
    number[num_idx] = 0;
    int val = atoi(number);
    if (val % 5 == 0 || val % 6 == 0) {
      printf("%d\n", val);
    }
  }
}

int
main(int argc, char *argv[]) {
  if (argc < 2) {
    fprintf(2, "Usage: sixfive <filename> [filename...]\n");
    exit(1);
  }

  for (int fi = 1; fi < argc; fi++) {
    char *fname = argv[fi];
    int fd = open(fname, 0);
    if (fd < 0) {
      fprintf(2, "sixfive: cannot open %s\n", fname);
      continue;
    }
    process_fd(fd);
    close(fd);
  }

  exit(0);
}

