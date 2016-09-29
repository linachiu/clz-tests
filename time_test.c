#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "contral.h"
#include <omp.h>

#define CLOCK_ID CLOCK_MONOTONIC_RAW
#define ONE_SEC 1000000000.0

int main(int argc, char const *argv[])
{
    struct timespec start = {0, 0};
    struct timespec end = {0, 0};

    if (argc < 2) return -1;

    int i, loop = 25;

    // Baseline
    clock_gettime(CLOCK_ID, &start);
    for(uint32_t i = 0; i <= 0x00FFFFFF; ++i) {
        clz_bin(i);
    }
    clock_gettime(CLOCK_ID, &end);
    printf("%lf,", (double) (end.tv_sec - start.tv_sec) +
           (end.tv_nsec - start.tv_nsec)/ONE_SEC);


    // OpenMP with 2 threads
    clock_gettime(CLOCK_ID, &start);
    for(uint32_t i = 0; i <= 0x00FFFFFF; ++i) {
        clz(i);
    }
    clock_gettime(CLOCK_ID, &end);
    printf("%lf,", (double) (end.tv_sec - start.tv_sec) +
           (end.tv_nsec - start.tv_nsec)/ONE_SEC);

    return 0;
}
