#include <stdio.h>
#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>


// //the program should print hello wvery 5 seconds until CTRL-C is pressed, then when CTRL-C is pressed it should print finished
// int done =0; // initially should be set to 1 to allow the while loop to be infinit
//
// void sig_handler(int sigNo) {
//     if (sigNo == SIGINT) { // here we will replace with SIGALRM
//         printf("Hello\n");
//     }
//     else {
//         printf("Finished\n");
//         done = 1; // done, being a bool-like variable should be set to 0 when we are done
//     }
// }
//
// int main(int argc, char *argv[]) {
//     signal(SIGINT, sig_handler);
//     signal(SIGALRM, sig_handler);
//     alarm(5); // this should be in the while loop otherwise only one alaram is sent
//     while(done) {
//         sleep(1); // here we should put sleep to 5, so each while iteration creates the alarm and wait for it to be sent
//     }
// }



int done =1;

void sig_handler(int sigNo) {
    if (sigNo == SIGALRM) {
        printf("Hello\n");
    }
    else {
        printf("Finished\n");
        done = 0;
    }
}

int main(int argc, char *argv[]) {
    signal(SIGINT, sig_handler);
    signal(SIGALRM, sig_handler);
    while(done) {
        alarm(5);
        sleep(5);
    }
}
