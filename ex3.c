#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

// int main(void) {
//     char v[512];
//     unsigned long fd = open("/my/OF/videos/fooling_around.mp4", O_RDONLY);
//     read(fd, v, 511);
//     printf("File contents: %s", v);
//     close(fd);
// }

//the code is mostly allright, there are a few small problems. first, we could intialize the buffer v
//fd could be an int to avoid any truncation
//it must be mentioned that this program will only read the first 511 bytes of data, not the whole content
//the data read will obviuslty be an ecoding in binary
//fail checks for the open, read, close could be added

int main(void) {
    char v[512] = {};
    int fd = open("/my/OF/videos/fooling_around.mp4", O_RDONLY);
    if (fd < 0) {
        perror("open");
        exit(1);
    }
    int n;
    while ((n=read(fd, v, 511))>0) {
        printf("File contents: %s", v);
    }
   if ( close(fd)<0)
       perror("close");
    return 0;
}