#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <wait.h>
#include <unistd.h>


// . Write a C program for UNIX that creates 20 processes (including the parent). Each
// process prints 10 lines containing its type and PID. Afterward, the child processes
// terminate with different values, and the parent prints the values returned by the
// children.

int main(void) {
    for (int i=0; i<20; i++) {
        const pid_t pid = fork();
        if (pid == 0) {
            for (int j=0; j<10; j++) {
                printf("Child process number %d with pid: %d\n", i+1 ,getpid());
            }
            exit(i+1);
        }
        if (pid == -1) {
            perror("fork failed for one of the children");
            exit(0); // a value outside rage 1-20 which we allocate for the child processes
        }
        if (pid > 0) {
            int status;
            wait(&status);
            if (WIFEXITED(status)) {
                printf("Parent says: Child process with pid ended with exit status: %d\n", WEXITSTATUS(status));
            }
        }
    }

    for (int i=0; i<10; i++) {
        printf("Parent with pid: %d\n", getpid());
    }

    return 0;
}