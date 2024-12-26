#include <sys/wait.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>

// int main(void) { // the program is supposed to make a child print Hello World and the parent to wait for the child
//     pid_t pid = fork();
//     char &n;
//     if (pid > 0) { // this would indicate the parent, but that don' make sense. Additionaly after printinh hello world the process can be exited
//         printf("Hello World!\n");
//     }
//     wait(n); // here we could make n an int to avoid useless casting
//     printf("Child has finished\n");
//     return 0;
// }

int main(void) {
    pid_t pid = fork();
    int n;
    if (pid == 0) {
        printf("Hello World!\n");
        exit(0);
    }
    wait(&n); // here we could make n an int to avoid useless casting
    printf("Child has finished\n");
    return 0;
}
