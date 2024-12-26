#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>


// 7. Explain the effect of the following code:
// int i;
// for (i = 1; i <= 10; i++)
//     fork();
int main(void) {
    int i;
    for (i = 1; i <= 10; i++)
         fork();
    // basically this code create 2^10 processes as each child process created will renter the loop and create procceses of it s own
    //In the first iteration, 2 processes exist (the original parent and one child).
    //In the second iteration, each of these 2 processes forks again, resulting in 4 processes.
    //This doubling continues for every iteration, leading to 2^10=1024 processes by the end of the loop.
    return 0;
}
