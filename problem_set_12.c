#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/stat.h>
#include <sys/wait.h>

void reverse(char* c) {
    int first = 0;
    int last = strlen(c) - 1;
    if (last > 0 && c[last] == '\n') {
        c[last] = '\0';
        last--;
    }
    while (first <= last) {
        char temp = c[first];
        c[first] = c[last];
        c[last] = temp;
        first++;
        last--;
    }
}

// Implement a C application that receives 1 command line argument, representing a file path.
// The application should be able to spawn a subprocess.

// a) The main process will read the content of the file and send (write) the content through a pipe to the child process in reverse order.
int main(int argc, char **argv) {
    if (argc != 2) {
        printf("Usage: %s <file>\n", argv[0]);
        exit(1);
    }
    int daddy_pipe[2];
    if (pipe(daddy_pipe) < 0)
        perror("pipe fucking failed metal");
    pid_t pid = fork();
    if (pid <0) {
        perror("fork fucking failed ass fork");
        exit(1);
    }
    if (pid == 0) {
        close(daddy_pipe[1]);
        char reverse[1024] = {0};
        int nbytes = read(daddy_pipe[0], reverse, 1024); // here it wokrks only if the content is under 1024 bytes, if it s bigger than that
        //good luck babe, I have no idea, nor I can come up with on how to reverse the whole contents, well besides creating an array the size of file
        //but I ain't gonna do that
        if (nbytes < 0) {
            perror("read failed");
        }
        printf("The child prints %s\n", reverse);
        exit(0);
    }
    if (pid > 0) {
        close(daddy_pipe[0]);
        int fd = open(argv[1], O_RDONLY);
        if (fd < 0) {
            perror("File opening failed");
            exit(1);
        }
        char buf[1024]={};
        int nbytes = read(fd, buf, 1023);
        if (nbytes == 0) {
            perror("Unable to read file");
            close(fd);
            return 1;
        }
        buf[nbytes] = '\0';
        reverse(buf);
        write(daddy_pipe[1], buf, nbytes);
        close(fd);
        close(daddy_pipe[1]);
        wait(NULL);
    }

    return 0;
}