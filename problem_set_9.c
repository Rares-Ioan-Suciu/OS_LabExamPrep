#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <wait.h>
#include <unistd.h>


// Write a C application to recreate the same process hierarchy as depicted in a whiteboard illustration. Each process will print its PID and the parent’s PID
// If I remeber corectly, parent spawns a child, that also spawns 2 more, afterwatd parent has one more child. we wait for children to finish and the for parents side

int main(void){
    pid_t pid_child1 = fork();
    if(pid_child1  <0) {
        perror("Fork failed for child number 1");
        exit(0);
    }
    else if(pid_child1 == 0) {
        pid_t pid_child2 = fork();
        if(pid_child2 < 0) {
            perror("Fork failed for child number 2");
            exit(0);
        }
        else if(pid_child2 == 0) {
            pid_t pid_child3 = fork();
            if(pid_child3 < 0) {
                perror("Fork failed for child number 3");
                exit(0);
            }
            else if(pid_child3 == 0) {
                printf("I am child number 3 with %d and my parent's pid is: %d\n", getpid(), getppid());
                exit(3);
            }
            if (pid_child3 > 0) {
                wait(NULL);
                printf("I am child number 2 with %d and my parent's pid is: %d\n", getpid(), getppid());
                exit(2);
            }
        }
       if(pid_child2 > 0) {
           wait(NULL);
           printf("I am child number 1 with %d and my parent's pid is: %d\n", getpid(), getppid());
           exit(1);
       }
    }
    if (pid_child1 >0) {
        wait(NULL);

        pid_t pid_parent_child = fork();
        if(pid_parent_child < 0) {
            perror("Fork failed for parent number");
            exit(0);
        }
        else if(pid_parent_child == 0) {
            printf("I am parent 2 with pid %d and my parent's pid is: %d\n", getpid(), getppid());
            exit(2);
        }
        wait(NULL);
        printf("I am parent and my pid is %d, and whomever my parent is has pid: %d\n", getpid(), getppid());
    }
}