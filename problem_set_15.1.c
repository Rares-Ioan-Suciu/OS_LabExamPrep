#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <signal.h>

// 1. Create a C application that creates a subprocess.
// The parent will send the son the SIGUSR1 signal every 1 second.The son computes a[n+1] = a[n] + 1/r, a[0] = a0
// Every time the SIGUSR1 is received, the son will print the current value of a[n].
// After 2 prints, the son ends its execution.
// The parent first prints *****; afterwards, each second will alternate between +++++ and ------.
// The parent will finish after getting the son's exit code and printing it.
// a0 and r are received at execution.

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <sys/wait.h>
#include <stdlib.h>

double a;
double r;
int count; // make all of this global so we can use it from outside

void sigusr_handler(int sig) {
    printf("The value of a[n] at this time = %.6f\n", a);
    count++;
}


int main(int argc, char* argv[])
{
    int status;
    if (argc != 3) {
        printf("Usage: %s <a>, <r>\n", argv[0]);
        exit(0);
    }
    a = atof(argv[1]);
    r = atof(argv[2]);

    if (signal(SIGUSR1, SIG_IGN)== SIG_ERR) {
        fprintf(stderr, "Error in signal handler\n");
        exit(1);
    };

    pid_t child_pid;
    if ((child_pid = fork()) < 0)
    {
        printf("Error creating child process\n");
        exit(2);
    }

    if (child_pid == 0)
    {
        if (signal(SIGUSR1, sigusr_handler)== SIG_ERR) {
            fprintf(stderr, "Error in signal handler\n");
            exit(3);
        }
        while (count < 2) {
            a = a + 1.0/r;
            //printf("The value of a %.6f\n", a);
        }
        exit(123);
    }
    else
    {
        printf("*****\n");
        int status;
        sleep(1);
        kill(child_pid, SIGUSR1);
        printf("+++++\n");
        sleep(1);
        kill(child_pid, SIGUSR1);
        printf("------\n");

        pid_t result = waitpid(child_pid, &status, WNOHANG);
            if (result > 0) {
                if (WIFEXITED(status)) {
                    printf("Child exited with code %d\n", WEXITSTATUS(status));
                }
            }

    }
    return 0;
}