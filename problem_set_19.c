#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <pthread.h>
#include <semaphore.h>
#include <tgmath.h>

// Write a C program consisting of a single process in which 6 threads of execution (including the initial one) run.
// The process creates an array of 5 characters, whose elements are initialized with '#'.
// There will be 5 "producer" threads in the process that will continuously fill in a random location in the array with one character each,
// as follows: the first thread with the character 'A', the second with 'B' and so on up to 'E'. Each "producer" thread will count how many
// characters it has entered in the array; the first thread will terminate when it has entered 100,000 characters,
// the second 200,000 and so on. The main thread will continuously display the contents of the array until all the "producer" threads terminate,
// at which point it will take over their state and terminate the process.
// Hint: to announce the termination of the "producer" threads, each of them will set a distinct global variable, the main thread testing all these variables.


int finished_all = 0;
int finished_threads[5] = {0,0,0,0,0};

char displayer[5] = {'#','#','#','#','#'};
char resp_char[5] = {'A', 'B', 'C', 'D', 'E'};


void* work_girl(void* arg) {
    const int id = *((int*)arg) ;
   // printf("Girl ID: %d\n", id);
    int pers_count = 0;
    char c = resp_char[id];

    while (pers_count < (id+1) * 100000) {
        int index = rand() % 5;
        displayer[index] = c;
        pers_count++;
    }

    finished_threads[id] = 1;
    //printf("Thread %d finished\n", id);
    return NULL;
}



int main(void) {

    pthread_t threads[5];

    for (int i = 0; i < 5; i++) {
        int *id = malloc(sizeof(int));  // sending the id can cause a lot of problems, it ' s better that
        //each thread has it's own personal variable fro the id, either like this, or with a thread_is[] vector
        *id = i;
        if (pthread_create(&threads[i], NULL, work_girl, id))
        {
            perror("pthread_create");
            exit(1);
        }
    }

    while (!finished_all) {
        for (int i = 0; i < 5; i++) {
            finished_all = 1;
            finished_all *= finished_threads[i];
        }

        for (int i = 0; i < 5; i++) {
             printf("%c", displayer[i]);
        }
        printf("\n");
    }


    for (int i = 0; i < 5; i++) {
        if (pthread_join(threads[i], NULL)) {
            perror("pthread_join");
            exit(1);
        }
    }

    printf("All done\n");
    return 0;
}