#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <wait.h>
#include <unistd.h>

// 10. Write a C application that takes an integer argument (N1) from the command line,
// forks two child processes, and computes:
// (a) The sum of integers up to N1 (first child).
// (b) The factorial of N1 (second child).
// The parent waits for both children to finish, then prints ”Done”

void first_sum(const int n) {
    printf("[%d] Sum of the first %d integers is: %d\n", getpid(), n, n*(n+1)/2);
}

int factorial(const int n) {
    if (n == 0 || n == 1) {
        return 1;
    }
    return n*factorial(n-1);
}

int main(int argc, char **argv) {
    if (argc != 2) {
        printf("Usage: %s <integer n1>\n", argv[0]);
        exit(1);
    }
    int n1 = atoi(argv[1]);
    const pid_t pid1 = fork();
    if (pid1 < 0) {
        perror("fork failed");
        exit(1);
    }
    if (pid1 == 0) {
        first_sum(n1);
        exit(0);
    }
    const pid_t pid2 = fork();
    if (pid2 < 0) {
        perror("fork failed for child 2");
    }
    if (pid2 == 0) {
        printf("[%d] Child 2 computes factorial of the first %d integers as: %d\n",getpid(), n1, factorial(n1));
        exit(1);
    }
    wait(NULL);
    printf("[%d] DONE\n", getpid());
}