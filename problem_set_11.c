#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <wait.h>
#include <unistd.h>



// Modify the program from set 10 so that each child process spawns subchildren. Each subchild will exit with its respective index value (1, 2, ..., N). The main process's children will then calculate both the sum and the product of the exit values from their respective subchildren, and each main process's child will exit with these computed results.
// The parent process will wait for the two children to terminate, capture their exit statuses, and print the computed sum and product values from both children..
//


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
        int sum = 0;
        for (int i = 1; i <= n1; i++){
            pid_t subchild1 = fork();
            if (subchild1 == 0) {
                exit(i);
            }
            else if (subchild1 > 1) {
                int status;
                waitpid(subchild1, &status, 0);
                if (WIFEXITED(status)) {
                    sum += WEXITSTATUS(status);
                }
            } else {
                perror("fork of subchild failed");
            }
        }
        exit(sum);
    }
    const pid_t pid2 = fork();
    if (pid2 < 0) {
        perror("fork failed for child 2");
    }
    if (pid2 == 0) {
        int fact = 1;
        for (int i = 1; i <= n1; i++) {
            pid_t subchild2 = fork();
            if (subchild2 == 0) {
                exit(i);
            }
            else if (subchild2 > 1) {
                int status;
                waitpid(subchild2, &status, 0);
                if (WIFEXITED(status)) {
                    fact *= WEXITSTATUS(status);
                }
            } else {
                perror("fork of subchild failed");
            }
        }
        exit(fact);
    }
    int sum, factorial;
    waitpid(pid1, &sum, 0);
    waitpid(pid2, &factorial, 0);
    if(WIFEXITED(sum) && WEXITSTATUS(factorial)) {
        printf("C1[%d] gives us the sum %d\nC2[%d] gives us the factorial %d\n", pid1, WEXITSTATUS(sum), pid2, WEXITSTATUS(factorial));
        //must be rembered babes that the status of exist is rep on a single byte so don't use this for values too big, we ll use pipes for that
    }
}