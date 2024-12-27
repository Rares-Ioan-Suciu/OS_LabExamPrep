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

// 1. Solve the problem with the statement below using known IPC mechanisms: shared memory or semaphores.
// Consider a barbershop staffed by a barber. He has five chairs in the waiting room.
// The barber can only serve one customer at a time, and in a customer crisis the barber
// is idle (the process that simulates the barber is blocked; see below).
// A customer, depending on the situation in the barbershop at the time of arrival, can act in one of the following ways:
// if the barber is sleeping, wake him up and ask to be shaved;
// if the barber is busy, but there are free chairs in the waiting room, take a chair and wait for your turn;
// if the waiting room is full then the customer leaves (unsatisfied!).
// 2. Write a program that simulates the above described processes: the barber and the customers are distinct processes,
// the number of customers can be given as a parameter.
// The program will also display a statistic of the customers served and the dissatisfied ones, at the end.

sem_t *idle_barber;
sem_t *serveABLE;
sem_t *entering;

int *waiting_customers;
const int seats = 5;

int *dissatisfied;
int *satisfied;

void barber_process() {
    while (1) {
        sem_wait(serveABLE); // waiting for a cusotmer to be ready to be served

        sem_wait(entering);
        (*waiting_customers)--;
        printf("Barber: Serving. Waiting customers: %d\n", *waiting_customers);
        sem_post(entering); // after serving a new customer can come in

        sleep(5); // haircut takes 5 seconds
        sem_post(idle_barber);
        (*satisfied)++;
    }
}

void customer_process(int pid) {
    sem_wait(entering);

    if (*waiting_customers < seats) { // only accept customer if we have seats
        (*waiting_customers)++;
        printf("Customer %d: Took a seat. Waiting customers: %d\n", pid, *waiting_customers);

        sem_post(serveABLE);   // signal to the barber that a customer is ready to be seRVed
        sem_post(entering);    // a new customer can enter
        sem_wait(idle_barber);
        printf("Customer %d: Finished haircut.\n", pid);
    } else {
        printf("Customer %d: No seats available. Leaving unsatisfied.\n", pid); // if no seats leave un-served
        (*dissatisfied)++;
        sem_post(entering); // a new customer can enter
    }

    exit(0);
}

int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <number_of_customers>\n", argv[0]);
        return 1;
    }

    int number_customers = atoi(argv[1]);
    if (number_customers < 1) {
        fprintf(stderr, "Number of customers must be a positive integer.\n");
        return 1;
    }


    waiting_customers = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    satisfied = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    dissatisfied = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);

    *waiting_customers = 0;
    *satisfied = 0;
    *dissatisfied = 0;


    sem_unlink("/barber_ready");
    sem_unlink("/customer_ready");
    sem_unlink("/access_waiting_room");

    idle_barber = sem_open("/barber_ready", O_CREAT, 0644, 0);
    serveABLE = sem_open("/customer_ready", O_CREAT, 0644, 0);
    entering = sem_open("/access_waiting_room", O_CREAT, 0644, 1);

    if (idle_barber == SEM_FAILED || serveABLE == SEM_FAILED || entering == SEM_FAILED) {
        perror("Semaphore initialization failed");
        return 1;
    }

    const pid_t barber = fork();
    if (barber == -1) {
        perror("Failed to fork barber process");
        return 1;
    }

    if (barber == 0) {
        barber_process();
    }

    srand(time(NULL));
    for (int i = 0; i < number_customers; i++) { // create a custimer once every max 2 seconds and try to serve him
        pid_t customer = fork();
        if (customer == -1) {
            perror("Failed to fork customer process");
            return 1;
        }

        if (customer == 0) {
            customer_process(getpid());
        }

        sleep(rand() % 2 + 1);
    }


    for (int i = 0; i < number_customers; i++) {
        wait(NULL);
    }

    kill(barber, SIGKILL);
    wait(NULL);


    printf("Barber terminated.\n");
    printf("Number of customers satisfied: %d\n", *satisfied);
    printf("Number of customers dissatisfied: %d\n", *dissatisfied);
    printf("Satisfaction rate: %.2f%%\n", (*satisfied * 100.0) / number_customers);


    sem_close(idle_barber);
    sem_close(serveABLE);
    sem_close(entering);
    sem_unlink("/barber_ready");
    sem_unlink("/customer_ready");
    sem_unlink("/access_waiting_room");
    munmap(waiting_customers, sizeof(int));
    munmap(satisfied, sizeof(int));
    munmap(dissatisfied, sizeof(int));

    return 0;
}
