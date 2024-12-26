#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// int main(int argv, char** argc) { // there is no problem, besides readabilty, with this reversed naming of the argument count and argument value
//     unsigned long pid = fork(); // should be int, having some problems otherwise in case of errors
//     if (pid == 0) {
//         printf("First argument is: %s\n", argv[1]);// here argv would hold the number of arguments, so it no't even a vector
//     }
//     return 0;
// }

int main(int argc, char** argv) {
    unsigned long pid = fork();
    if (pid == 0) {
        printf("First argument is: %s\n", argv[1]);
    }
    return 0;
}