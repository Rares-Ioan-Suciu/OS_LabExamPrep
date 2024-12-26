#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <ctype.h>

// Implement an application based on the example from http://www.ciprianpungila.
// com/uvt/so2011/lab2/, which modifies the behavior of the -a, -b, and -c options
// such that each of them can compute option arguments. The program should calculate the sum of all option arguments. For example:
// <path to exec>/a.out -a 12 -b 24 -c 12
// Should print:
// 48 (12 + 24 + 12)


int main (int argc, char **argv) {
    char *avalue = NULL;
    char *bvalue = NULL;
    char *cvalue = NULL; // we give up on the a and b flags, and we use avalue and bvalue to get their optarg
    int a = 0, b = 0, c = 0; // will store here the actual values
    int index;
    int opt;

    opterr = 0; // put errors of opt on off

    while ((opt = getopt(argc, argv, "a:b:c:")) != -1) { // a:b:c: are off
        switch (opt) {
            case 'a':
                avalue = optarg;
                a = atoi(avalue);// to all of these 3 cases we could add a form of sanitization to make sure we have an int, but fuck it
                break;
            case 'b':
                bvalue = optarg;
                b = atoi(bvalue);
                break;
            case 'c':
                cvalue = optarg;
                c = atoi(cvalue);
                break;
            case '?':
                if (optopt == 'c' || optopt == 'a' || optopt == 'b') {
                    fprintf(stderr, "Option -%c requires an argument.\n", optopt);
                } else if (isprint(optopt)) {
                    fprintf(stderr, "Unknown option `-%c'.\n", optopt);
                } else {
                    fprintf(stderr, "Unknown option character `\\x%x'.\n", optopt);
                }
            return 1;
            default:
                abort();
        }
    }

    printf("aflag = %d, bflag = %d, cvalue = %d\n", a, b, c); // we print the values just for funnsies, I hate this problesm

    printf("The sum is: %d\n", a + b + c); // print sum

    for (index = optind; index < argc; index++) {
        printf("Non-option argument %s\n", argv[index]); // print all the arguments that are not a,b,c
    }

    return 0;
}

