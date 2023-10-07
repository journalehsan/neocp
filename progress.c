/*
 * Simple progress bar implementation in C. 
 *
 *
 */
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>



int main(int argc, char **argv) {
    int i;
    double amount = 0.0;
    uint64_t start = get_timestamp();

    for (i = 0; i < 10000; i++) {
        amount += 0.01;
        print_progress(amount, start);
        usleep(2000000 / (i + 1));
    }

    printf("\n");
}
