#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <signal.h>
//
// 2. Create a C application that switches between executing two processes.T
// the parent will start both processes at once but will allow only the first subprocess to run for a
// number of N seconds. After that, the process will be put on hold (wait), and the second subprocess run for a number of N seconds.
// Then the parent process will stop the second process and resume the first one.
// Each process switch will be accompanied by a message (e.g.: "Process x was put on hold, Process y started").
// This cycle will be done until the parent receives the SIGUSR1 signal.

pid_t child_pid1;
pid_t child_pid2;

void sig_pause_C1(int sig) {
    printf("Child 1 (%d) is put on pause, child 2 (%d) starts again\n", getpid(), child_pid2);
    raise(SIGSTOP);
}
void sig_pause_C2(int sig) {
    printf("Child 2 (%d) is put on pause, child 1 (%d) starts again\n", getpid(), child_pid1);
    raise(SIGSTOP);
}
void sig_sigusr1(int sig) {
    printf("The switch has finished. All processes are ending. Bye babes\n");
    kill(child_pid1, SIGKILL);
    kill(child_pid2, SIGKILL);
    exit(0);
}

int main(const int argc, char* argv[])
{
    if(argc != 2) {
        printf("Usage: %s <positive integer>\n", argv[0]);
        exit(1);
    }
    int N = atoi(argv[1]);

    int fd[2];
    if(pipe(fd) < 0) {
        printf("Pipe failed\n");
        exit(1);
    }

    signal(SIGUSR1, SIG_IGN);
    signal(SIGTSTP, SIG_IGN);
    signal(SIGCONT, SIG_IGN); // we put on ignore all the singals that we will use so no accidental signal handling can happen

    child_pid1 = fork();
    if (child_pid1 < 0) {
        perror("fork error in child 1");
        exit(1);
    }
    if (child_pid1 == 0) {
        signal(SIGTSTP, sig_pause_C1);
        close(fd[1]);
        read(fd[0], &child_pid2, sizeof(child_pid2));
        close(fd[0]);
        while (1) {}
    }

    child_pid2 = fork();
    if (child_pid2 < 0) {
        perror("fork error in child 2");
        exit(1);
    }
    if (child_pid2 == 0) {
        signal(SIGTSTP, sig_pause_C2);
        close(fd[0]);
        pid_t child_pid = getpid();
        write(fd[1], &child_pid, sizeof(getpid()));
        close(fd[1]);
        while (1) {}
    }

    kill(child_pid2, SIGTSTP);
    printf("Initial setup: child 1 (%d) is running, child 2 (%d) is paused.\n", child_pid1, child_pid2);



    signal(SIGUSR1, sig_sigusr1);
    while (1) {
        kill(child_pid1, SIGTSTP);
        kill(child_pid2, SIGCONT);
        sleep(N);
        kill(child_pid2, SIGTSTP);
        kill(child_pid1, SIGCONT);
        sleep(N);
    }

    close(fd[0]);
    close(fd[1]);
    return 0;
}