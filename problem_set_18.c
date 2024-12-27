#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <pthread.h>
#include <semaphore.h>
#include <tgmath.h>

// Write a C program that spawns 6 threads to find prime numbers between 2 and
// 10,000,000 that end in 7 or 9, limiting concurrency to 3 threads at a time using a
// semaphore.

sem_t cunty_sem;
long total_count = 0;

int is_prime(const int n) {
    if (n < 2) {
        return 0;
    }
    if (n == 2) {
        return 1;
    }
    if (n % 2 == 0) {
        return 0;
    }
    for (int i = 3; i < sqrt(n); i += 2) {
        if (n % i == 0) {
            return 0;
        }
    }
    return 1;
}

void *worker_thread(void *arg) {
    const int id = *((int*)arg);
    int pers_count = 0;


    const int total_numbers = 10000000;
    const int range = total_numbers / 6;
    const int inferior_limit = range * (id - 1);
    const int superior_limit = (id == 6) ? total_numbers : range * id;

    sem_wait(&cunty_sem);

    for (int i = inferior_limit; i < superior_limit; i++) {
        if (is_prime(i) && (i % 10 == 7 || i % 10 == 9)) {
            pers_count++;
        }
    }


    total_count += pers_count;
    sem_post(&cunty_sem);

    printf("Thread %d found this many: %d\n", id, pers_count);

    return NULL;
}

int main(void) {

    pthread_t thread[6];

    if (sem_init(&cunty_sem, 0, 3)) {
        perror("sem_init failed");
        exit(1);
    }

    for (int i = 0; i < 6; i++) {
        if (pthread_create(&thread[i], NULL, worker_thread, &i)) {
            perror("pthread_create error");
            exit(1);
        }
    }

    for (int i = 0; i < 6; i++) {
        if (pthread_join(thread[i], NULL)) {
            perror("pthread_join error");
            exit(1);
        }
    }

    printf("In total we found %ld primes ending in 7 and 9, in the interval given\n", total_count);
    sem_destroy(&cunty_sem);
    return 0;
}