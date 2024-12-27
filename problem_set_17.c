#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <wait.h>
#include <time.h>
#include <pthread.h>
#include <semaphore.h>

// 17. Write a C++ program that creates three child processes:
// • Child 1: Sends its PID to Child 2 using a pipe.
// • Child 2: Reads Child 1’s PID and prints it.
// • Child 3: Prints its parent’s PID.

int main(void) {
    int daddy_pipe[2];

    pipe(daddy_pipe); // NU UITA SA SI FACI PIPE-UL NU DOAR SA L CREEZI

    pid_t child_pid1;
    pid_t child_pid2;
    pid_t child_pid3;

    child_pid1 = fork();
    if (child_pid1 < 0) {
        perror("fork for child 1 failed");
        exit(1);
    }
    if (child_pid1 == 0) {
        pid_t pid = getpid();
        close(daddy_pipe[0]);
        write(daddy_pipe[1], &pid, sizeof(pid_t));
        close(daddy_pipe[1]);
        exit(0);
    }

    child_pid2 = fork();
    if (child_pid2 < 0) {
        perror("fork for child 2 failed");
        exit(1);
    }
    if (child_pid2 == 0) {
        pid_t pid;
        close(daddy_pipe[1]);
        read(daddy_pipe[0], &pid, sizeof(pid_t));
        close(daddy_pipe[0]);
        printf("Child 2 (%d) says child 1 has %d pid\n", getpid(), pid);
        exit(0);
    }

    wait(NULL);
    child_pid3 = fork();
    if (child_pid3 < 0) {
        perror("fork for child 3 failed");
        exit(1);
    }
    if (child_pid3 == 0) {
        printf("Child 3 says daddy has a pipe of this size: %d\n", getppid());
        exit(0);
    }

    wait(NULL);
    printf("All tasks done");
    return 0;
}