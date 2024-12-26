#include <ctype.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

// int main(int argc, const char argv[][]) { //the dimensions for the second [] have to be mentioned, much better to use char*[]
//     int f  = open(argv[1], O_WRONLY), c, total; // does not make sense to open only on Write Only if we want to read it's content
//     //we could replace it with O_RDONLY
//     //total is also not initalized to 0, c could be an unsigned char
//     if (f==0) // if it's 0 then it links to the standard input, doesn't make sense if we want to read from a file f>2
//     {
//           while ((n=read(f, &c, 1)>0)) // n is not declared, also a parenthesis is missing
//         {
//             if (isalpha(c)) // is alpha will only look at letters, we should also use isdigit() to count for digits
//                 total ++;
//         }
//         close(f);
//     }
//     printf("Found %ud alphanumeric elements in file!\n", total);
// }

int main(int argc, const char *argv[]) {
    const int f = open(argv[1], O_RDONLY);
    char c;
    int total = 0;


    if (f > 2) {
        int n;
        while ((n = read(f, &c, 1)) > 0) {  // be really careful with the parenthesis
            if (isalpha(c) || isdigit(c))
                total++;
        }
        close(f);
    } else {
        perror("Error opening file");
    }

    printf("Found %d alphanumeric elements in file!\n", total);
    return 0;
}