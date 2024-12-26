#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <dirent.h>
#include <ctype.h>
#include <string.h>

// Implement a basic cp-like command that copies the contents of one file to another.
// Files are specified as arguments to the program (only 2 arguments are allowed).

int main(const int argc, const char **argv) {
    if (argc != 3) {
        printf("Usage: %s <source_file> <new_file>\n", argv[0]);
    }
    int fd_src = open(argv[1], O_RDONLY);
    if (fd_src == -1) {
        perror("error opening source file");
        exit(1);
    }
    int fd_des = open(argv[2], O_RDWR | O_CREAT | O_TRUNC, 0777);
    if (fd_des == -1) {
        perror("error opening destination file");
        close(fd_src);
        exit(1);
    }

    char buf[1024];
    int n;
    while ((n = read(fd_src, buf, sizeof(buf))) > 0) {
        if (write(fd_des, buf, n)<0) {
            perror("error writing to destination file");
            close(fd_des);
            close(fd_src);
            exit(1);
        }

    }
    if (n == -1) {
        perror("error reading source file");
        exit(1);
    }
    if (close(fd_src) == -1) {
        perror("error closing source file");
        exit(1);
    }
    if (close(fd_des) == -1) {
        perror("error closing destination file");
        exit(1);
    }

    return 0;
}