#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <unistd.h>
#include <math.h>

// Semaphore for controlling concurrency
sem_t sem;
// Shared variable to count primes ending in 7 or 9
int number_primes = 0;

// Function to check if a number is prime
int is_prime(int n) {
    if (n < 2) return 0;        // Numbers < 2 are not prime
    if (n == 2) return 0;       // 2 is prime but does not end in 7 or 9
    if (n % 2 == 0) return 0;   // Exclude even numbers > 2

    for (int i = 3; i <= pow(n,0.5); i += 2) {
        if (n % i == 0) {
            return 0;
        }
    }
    return 1;
}

// Worker thread function
void* worker_thread(void* arg) {
    const int id = *((int*)arg);
    int count_primes = 0;

    const int total_numbers = 10000000;
    const int range = total_numbers / 6;
    const int inferior_limit = range * (id - 1);
    const int superior_limit = (id == 6) ? total_numbers : range * id;

    sem_wait(&sem);

    for (int i = inferior_limit; i < superior_limit; i++) {
        if (is_prime(i) && (i % 10 == 7 || i % 10 == 9)) {
            count_primes++;
        }
    }


    number_primes += count_primes;
    sem_post(&sem);

    printf("Worker thread %d has finished. Found %d primes ending in 7 or 9.\n", id, count_primes);
    return NULL;
}

int main() {
    // Initialize semaphore to allow at most 3 threads concurrently
    if (sem_init(&sem, 0, 3) != 0) {
        perror("sem_init failed");
        exit(1);
    }

    pthread_t threads[6]; // Create 6 threads
    int thread_ids[6];

    // Create threads
    for (int i = 0; i < 6; ++i) {
        thread_ids[i] = i + 1;
        if (pthread_create(&threads[i], NULL, worker_thread, &thread_ids[i]) != 0) {
            perror("pthread_create failed");
            exit(1);
        }
    }

    // Wait for all threads to finish
    for (int i = 0; i < 6; ++i) {
        pthread_join(threads[i], NULL);
    }

    // Destroy semaphore
    sem_destroy(&sem);

    printf("All threads have finished. Total number of primes ending in 7 or 9: %d.\n", number_primes);
    return 0;
}
