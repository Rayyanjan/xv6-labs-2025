/* minimal stub to satisfy Makefile while we test uptime */
#include "kernel/types.h"
#include "user/user.h"

void
memdump(char *fmt, char *data)
{
  /* minimal placeholder - restore real memdump later */
  (void)fmt; (void)data;
  printf(\"memdump: stub\\n\");
}

int
main(int argc, char **argv)
{
  /* simple placeholder program so linking succeeds */
  if (argc == 1) {
    printf(\"memdump: stub (no args)\\n\");
  } else {
    printf(\"memdump: stub arg=%s\\n\", argv[1]);
  }
  exit(0);
}
