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

void contentFormatter(char* content, size_t length) {
    int N = 0, M = 0, L = 0, K = 0;
    char* digits = malloc(length + 1);
    char* upperCase = malloc(length + 1);
    char* lowerCase = malloc(length + 1);
    char* rest = malloc(length + 1);

    if (!digits || !upperCase || !lowerCase || !rest) {
        perror("Memory allocation failed");
        exit(1);
    }

    digits[0] = upperCase[0] = lowerCase[0] = rest[0] = '\0';


    for (size_t i = 0; i < length; i++) {
        if (isdigit(content[i])) {
            digits[N] = content[i];
            N++;
        } else if (isupper(content[i])) {
            upperCase[M] = content[i];
            M++;
        } else if (islower(content[i])) {
            lowerCase[L] = content[i];
            L++;
        } else {
            rest[K] = content[i];
            K++;
        }
    }

    digits[N] = upperCase[M] = lowerCase[L] = rest[K] = '\0';

    snprintf(content, 1024, "%d%s%d%s%d%s%s", N, digits, M, upperCase, L, lowerCase, rest);


    free(digits);
    free(upperCase);
    free(lowerCase);
    free(rest);
}
// I forgot to make a new projext so this is also problemset14
// Modify th implementation from set 12 so that the child process will send back information to the parent based on the following format:
//
// <int N><N chars><int M><M chars><int L><L chars><K chars>, where
//
// N will be the number of digits, followed by the actual digits themselves.
// M will be the number of uppercase letters, followed by the actual letters themselves.
// L will be the number of lowercase letters, followed by the actual letters themselves.
// K chars will indicate the rest of the information received (from the parent)

// Implement a C application that receives 1 command line argument, representing a file path.
// The application should be able to spawn two subprocesses (both children from the same parent).
//
// a) The main process will read the content of the file and send (write) the content through a pipe to the first child process
//
// b) The first child process will revert the received information and send the result to the second child.
//
// c) The second child process will send back information to the parent based on the following format:
//
// <int N><N chars><int M><M chars><int L><L chars><K chars>, where
//
// N will be the number of digits, followed by the actual digits themselves.
// M will be the number of uppercase letters, followed by the actual letters themselves.
// L will be the number of lowercase letters, followed by the actual letters themselves.
// K chars will indicate the rest of the information received (from the parent).
int main(int argc, char **argv) {
    if (argc != 2) {
        printf("Usage: %s <file>\n", argv[0]);
        exit(1);
    }
    int daddy_pipe[2], nicki_pipe[2], beyonce_pipe[2];

    if (pipe(daddy_pipe) < 0)
        perror("pipe fucking failed metal"), exit(1);

    if (pipe(nicki_pipe) < 0)
        perror("nicki pipe flopped"), exit(1);

    if (pipe(beyonce_pipe) < 0)
        perror("beyonce pipe flopped"), exit(1);
    pid_t pid = fork();
    if (pid <0) {
        perror("fork fucking failed ass fork");
        exit(1);
    }
    if (pid == 0) {
        close(daddy_pipe[1]);
        close(nicki_pipe[0]);
        close(nicki_pipe[1]);
        close(beyonce_pipe[0]);

        char reverse_content[1024] = {0};
        int nbytes = read(daddy_pipe[0], reverse_content, 1024);
        if (nbytes < 0) {
            perror("read failed");
        }
        printf("The child prints %s", reverse_content);

        reverse(reverse_content);
        write(beyonce_pipe[1], reverse_content, strlen(reverse_content));
        close(beyonce_pipe[1]);
        close(daddy_pipe[0]);
        exit(0);
    }
    if (pid > 0) {
        pid_t pid_child2 = fork();
        if (pid_child2 < 0) {
            perror("fork failed");
            exit(1);
        }
        if (pid_child2 == 0) {
            close(daddy_pipe[1]);
            close(daddy_pipe[0]);
            close(nicki_pipe[0]);
            close(beyonce_pipe[1]);

            char formatted_content[1024] = {0};
            int nbytes = read(beyonce_pipe[0], formatted_content, 1024);
            if (nbytes < 0) {
                perror("read failed");
            }
            printf("The child prints %s\n", formatted_content);

            contentFormatter(formatted_content, strlen(formatted_content));
            write(nicki_pipe[1], formatted_content, strlen(formatted_content));
            close(nicki_pipe[1]);
            close(beyonce_pipe[0]);
            exit(0);

        }

        close(daddy_pipe[0]);
        close(nicki_pipe[1]);
        close(beyonce_pipe[0]);
        close(beyonce_pipe[1]);


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
        write(daddy_pipe[1], buf, nbytes);
        close(fd);
        close(daddy_pipe[1]);

        wait(NULL);
        int bytes_read = read(nicki_pipe[0], buf, strlen(buf));
        if (bytes_read < 0) {
            perror("Unable to read file");
        }
        printf("The parent recieved: %s\n", buf);
        close(nicki_pipe[0]);
    }

    return 0;
}