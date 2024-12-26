#include <iostream>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>

int main() {
    int pfd[2];
    if (pipe(pfd) < 0) {
        perror("Error creating pipe");
        exit(1);
    }

    const pid_t child1 = fork();
    if (child1 < 0) {
        perror("Error forking child 1");
        exit(1);
    }

    if (child1 == 0) {
        close(pfd[0]);
        const pid_t my_pid = getpid();
        if (write(pfd[1], &my_pid, sizeof(my_pid)) < 0) {
            perror("Error writing to pipe");
        }
        close(pfd[1]);
        exit(0);
    }

    const pid_t child2 = fork();
    if (child2 < 0) {
        perror("Error forking child 2");
        exit(1);
    }

    if (child2 == 0) {
        close(pfd[1]);
        pid_t pid_child1;
        ssize_t n = read(pfd[0], &pid_child1, sizeof(pid_child1));
        if (n > 0) {
            printf("Child 2 says child 1 has this PID: %d\n", pid_child1);
        } else if (n == 0) {
            printf("Child 2: No data to read from pipe\n");
        } else {
            perror("Error reading from pipe");
        }
        close(pfd[0]);
        exit(0);
    }

    const pid_t child3 = fork();
    if (child3 < 0) {
        perror("Error forking child 3");
        exit(1);
    }

    if (child3 == 0) {

        printf("Child 3 says parent has this PID: %d\n", getppid());
        exit(0);
    }
    close(pfd[0]);
    close(pfd[1]);

    wait(NULL);

    return 0;
}
