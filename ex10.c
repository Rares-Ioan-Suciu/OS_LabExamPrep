#include <stdio.h>
#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <pthread.h>
#include <semaphore.h>

/*
10. In the code snippet below, both the parent and the child process should enter the critical region
through mutual exclusion (either the parent first, followed by the child, or the child first, followed
by the parent - but not at the same time). Is the code snippet working correctly? Explain your answer.
If necessary, rewrite the code accordingly. [7.5p]
*/


// oh boy are there problems with this code

// int main() {
//     unsigned long n = 0; // this is not needed
//     unsigned int s; // s will be made into a sempahore
//     pid_t pid;
//     sem_init(&n, 1); // *intilize binary sempahore with mutex with value 1* THE FUNCTION HAS 3 PARAMETERS
//     sem_down(s, 1); // *semaphore down by 1* THIS FUNCTION DOESN T EVEN EXIST
//     pid = fork();
//     if (pid == 0) {
//
//         printf("Child is in the critical region\n");
//         sem_up(s,1); // *semaphore up by 1 * ALSO DOESNT EXIST< IS THIS PSEUDOCODE?
//         printf("Child left the critical region\n");
//         return 0;
//     }
//     sem_up(s,1); // semaphore up by 1
//     printf("Parent left the critical region\n");
//     return 0;
// }

int main() {
    sem_t s;
    pid_t pid;
    sem_init(&s, 1, 1);
    pid = fork();
    if (pid == 0) {
        sem_wait(&s);
        printf("Child is in the critical region\n");
        sem_post(&s);
        printf("Child left the critical region\n");
        return 0;
    }
    sem_wait(&s);
    printf("Parent is in the critical region\n");
    sem_post(&s);
    printf("Parent left the critical region\n");
    return 0;
}