#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// int main(int argv, char** argc)
// {
//     int pid = fork();
//     if (pid == 0)
//         exit(260); // here we would have a truncation, exit status is represented on 1 byte
//     int status;
//     wait(&status);
//     printf("%d\n", WEXITSTATUS(status));
//     return 0;
// } // besides the truncation the only other problems would be with checking if there are any fails

int main(int argv, char** argc)
{
    int pid = fork();
    if (pid < 0) {
        perror("fork failed");
        exit(1);
    }
    if (pid == 0)
        exit(4); // here we would have a truncation, exit status is represented on 1 byte
    int status;
    wait(&status);
    if (WIFEXITED(status)) // check if the child exited normally
         printf("%d\n", WEXITSTATUS(status));
    return 0;
} // besides the truncation the only other problems would be with checking if there are any fails