// user/find.c
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void find(char *path, char *filename) {
    char buf[512], *p;
    int fd;
    struct stat st;
    struct dirent de;

    if ((fd = open(path, 0)) < 0) {
        fprintf(2, "find: cannot open %s\n", path);
        return;
    }

    if (fstat(fd, &st) < 0) {
        fprintf(2, "find: cannot stat %s\n", path);
        close(fd);
        return;
    }

    switch(st.type) {
    case T_FILE:
        p = path;
        for (char *q = path; *q; q++)
            if (*q == '/')
                p = q + 1;

        if (strcmp(p, filename) == 0) {
            printf("%s\n", path);
        }
        break;

    case T_DIR:
        strcpy(buf, path);
        p = buf + strlen(buf);
        if (buf[strlen(buf) - 1] != '/') {
            *p++ = '/';
            *p = 0;
        }

        while (read(fd, &de, sizeof(de)) == sizeof(de)) {
            if (de.inum == 0)
                continue;
            if (strcmp(de.name, ".") == 0 || strcmp(de.name, "..") == 0)
                continue;

            memmove(p, de.name, strlen(de.name) + 1);

            find(buf, filename);
        }
        break;
    }

    close(fd);
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(2, "Usage: find <path> <filename>\n");
        exit(1);
    }

    find(argv[1], argv[2]);
    exit(0);
}
