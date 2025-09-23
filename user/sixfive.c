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

int
main(int argc, char *argv[]) {
  if (argc != 2) {
    fprintf(2, "Usage: sixfive <filename>\n");
    exit(1);
  }

  int fd = open(argv[1], 0);
  if (fd < 0) {
    fprintf(2, "sixfive: cannot open %s\n", argv[1]);
    exit(1);
  }

  char buf[BUF_SIZE];
  int n;
  char number[16];
  int num_idx = 0;

  while ((n = read(fd, buf, sizeof(buf))) > 0) {
    for (int i = 0; i < n; i++) {
      char c = buf[i];
      if (c >= '0' && c <= '9') {
        if (num_idx < sizeof(number) - 1) {
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

  close(fd);
  exit(0);
}
