#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>

// unsigned int n, fd; // fd can be a signed int
// char buffer[SIZE - 1];
//
// ...
//
// while ((n = read(fd, buffer, SIZE)) > 0) { // here is not correct since SIZE is bigger then the buffer, we would get an overflow, it should be SIZE-2 to save space fro null_termiantor
//     printf("Bytes read: %d\n", n);
//     printf("Content: %s\n", buffer);
// }


#define SIZE 128  // Define the buffer size

int main() {
    const int fd = open("/home/rares655/Sem_3/OS1/Examen/Ex7/cmake-build-debug/file.txt", O_RDONLY);
    if (fd < 0) {
        perror("Error opening file");
        return 1;
    }

    char buffer[SIZE-1];
    int n;

    while ((n = read(fd, buffer, SIZE - 2)) > 0) {
        printf("Bytes read: %d\n", n);
        printf("Content: %s\n", buffer);
    }

    if (n < 0) {
        perror("Error reading file");
    }

    close(fd);  // Close the file
    return 0;
}