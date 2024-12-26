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
11. The following snippet should create 6 threads in order to efficiently parallelize a specific task,
and then terminate. Investigate the code, find any problems that might exist, and rewrite the code
accordingly. [7.5p]
*/

// void* start(void* arg) {
//     int ThreadID = *arg; // arg here is void, needs to be cast to int pointer first
//     printf("Thread %d executed\n", ThreadID);
//     // The actual work is performed here
//     return NULL;
// }
//
// int main() {
//     pthread_t t[5]; // make the size 6, to accomadate all threads plus one, otherwise we risk smashing
//     float i; // here i should be an int as it will be used to access indexes
//
//     for (i = 0; i < 6; i++) {
//
//         pthread_create(t[i], NULL, start, i); // first argumetn  is a pointer pass by refernce
//         pthread_join(t[i], NULL); // if we do the join immediatly then it s just sequential no parralelization, this should have it own for
//     }
//
//     printf("Threads finished!\n");
//     return 0;
// }

void* start(void* arg) {
    int ThreadID = *(int*)arg; // arg here is void, needs to be cast to int pointer first
    printf("Thread %d executed\n", ThreadID);
    return NULL;
}

int main() {
    pthread_t t[6];
    int i ;

    for (i = 0; i < 6; i++) {
        pthread_create(&t[i], NULL, start, &i);
    }
    for (i=0; i < 6; i++) {
        pthread_join(t[i], NULL);
    }
    printf("Threads finished!\n");
    return 0;
}