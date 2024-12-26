#include <sys/wait.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// the code should creat a child process, that sends it a text from keyboard and the child prints it
// int main(void) {
//     unsigned char txt[2048], fd[2], n; // to ensure no problem with truncation fd and n can  be made int with no problem
//     pid_t pid = fork();
//     pipe(fd); // we could check if pipe creation worked
//     !!!!! VERY IMPORTANT PIPE NEEDS TO ALWAYS BE CALLED BEFORE FORK()
//     if (pid == 0) { // *parent* <- this comment is misleading, as 0 indicate child
//         printf("Enter text: ");
//         scanf("%s", txt);
//         close(fd[0]);
//         write(fd[1], txt, strlen(txt));
//         close(fd[1]);
//         wait(&n);
//     }
//     else { //<-only having an else here creates some problems, as we can have the case where the fork () failed
//         close(fd[1]);
//         read(fd[0], txt, 2048);
//         close(fd[0]);
//         printf("Child recieved: %s\n", txt);
//     }
// }


int main(void) {
    unsigned char txt[2048];
    int fd[2], n;
    if (pipe(fd)<0)
        perror("pipe failed");
    const pid_t pid = fork();
    if (pid > 0) {
        printf("Enter text: ");
        scanf("%s", txt);
        close(fd[0]);
        write(fd[1], txt, strlen(txt)+1);
        close(fd[1]);
        wait(&n);
    }
    else if (pid == 0){
        close(fd[1]);
        read(fd[0], txt, 2048);
        close(fd[0]);
        printf("Child recieved: %s\n", txt);
    }
    else {
        perror("fork failed");
        exit(1);
    }
    return 0;
}
