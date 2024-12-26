#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

// unsigned char fd; // this won't give an compilation error, but file descriptors have a bigger range than unsigned char, so the risk of truncation is high, fd should be int
//
// int main(void) {
//     if ((fd = open("/home/rares655/Sem_3/OS1/Examen/Ex1/cmake-build-debug/myfile", O_EXCL) | O_TRUNC)) // here the is a problem with the parathesis as O_TRUNC becomes an bitwise or |
//         // additionally, the flags don t really make sense O_EXCL creates the file only if it doesn't exist already and O_TRUNC deletes content from the file
//         {
//             read(fd, &c, 4);   // c would store the first 4 bytes of data into c, but c is not intialized
//            close(fd);
//         }
//     close(fd); // the file is already closed inside the if, and also in case of fail of open, this close would also produce an error
//     return 0;
// }
//

// Now we will give an correct implementation, where we open a file, read it's content into c, which will be a c-string

int fd;

int main(void) {
    char *c = malloc(5);
    if ((fd = open("/home/rares655/Sem_3/OS1/Examen/Ex1/cmake-build-debug/myfile", O_RDONLY))){
            read(fd, c, 4);   // c would store the first 4 bytes of data into c, but c is not intialized
            close(fd);
        }
   printf("The first 4 bytes of the file are: %s", c);
    free(c);
    return 0;
}