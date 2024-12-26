#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>

// Add a fourth (-d) and a fifth (-e) option based on the code found in Chapter 6
// from http://ciprianpungila.com/uvt/so2016/lab2/.
// • When -d is enabled, the program computes the average of the first 100 prime
// numbers and prints the result.
// • The -e option requires an argument (-e evalue), and computes the average
// between the average of the first cvalue prime numbers and the average of the
// first evalue prime numbers.
// Example output for:
// <path to exec>/a.out -a -b -c5 -d -e 8
// aflag = 1, bflag = 1, cvalue = 5, avg of first 100 primes = <value>, evalue
// = 8, AVG(5.6, 9.625) = 7.6125


float is_prime(const int n) {
    if(n < 2) {
        return 0;
    }
    if(n == 2) {
        return 1;
    }
    if(n % 2 == 0) {
        return 0;
    }
    for(int i = 3; i < n/2; i += 2) {
        if(n % i == 0) {
            return 0;
        }
    }
    return 1;
}

float first_primes(int n) {
    float sum = 0;
    float count  = 0;
    int i = 2;
    while (count < n) {
        count += is_prime(i);
        if(is_prime(i)) {
            sum += i;
        }
        i++;
    }
    return sum/count;
}

int main(int argc, char **argv) {
    int aflag = 0;
    int bflag = 0;
    float avg_100_primes = 0.0;
    char *cvalue = NULL;
    char *evalue = NULL;
    int index;
    int opt;

    opterr = 0;

    while ((opt = getopt(argc, argv, "abc:de:")) != -1) {
        switch (opt) {
            case 'a':
                aflag = 1;
            break;
            case 'b':
                bflag = 1;
            break;
            case 'd':
                avg_100_primes = first_primes(100);
            break;
            case 'c':
                cvalue = optarg;
            break;
            case 'e':
                evalue = optarg;
            break;
            case '?':
                if (optopt == 'c' || optopt == 'e') {
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


    float avg_ce = 0.0;
    if (cvalue != NULL && evalue != NULL) {
        float avg_c = first_primes(atoi(cvalue));
        float avg_e = first_primes(atoi(evalue));
        avg_ce = (avg_c + avg_e) / 2;
        printf("AVG(%0.4f, %0.4f) = %0.4f\n", avg_c, avg_e, avg_ce);
    }

    printf("aflag = %d, bflag = %d, cvalue = %s ", aflag, bflag, cvalue);
    printf("Avg of first 100 primes = %.4f ", avg_100_primes);
    if (evalue != NULL) {
        printf("evalue = %s, avg_ce = %.4f\n", evalue, avg_ce);
    }

    for (index = optind; index < argc; index++) {
        printf("Non-option argument %s\n", argv[index]);
    }

    return 0;
}
