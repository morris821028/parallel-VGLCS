#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <assert.h>
#include <time.h>
#include "VGLCS.h"

#define RDTSC_START(cycles)                                                   \
    do {                                                                      \
        register unsigned cyc_high, cyc_low;                                  \
        __asm volatile(                                                       \
            "cpuid\n\t"                                                       \
            "rdtsc\n\t"                                                       \
            "mov %%edx, %0\n\t"                                               \
            "mov %%eax, %1\n\t"                                               \
            : "=r"(cyc_high), "=r"(cyc_low)::"%rax", "%rbx", "%rcx", "%rdx"); \
        (cycles) = ((uint64_t)cyc_high << 32) | cyc_low;                      \
    } while (0)

#define RDTSC_FINAL(cycles)                                                   \
    do {                                                                      \
        register unsigned cyc_high, cyc_low;                                  \
        __asm volatile(                                                       \
            "rdtscp\n\t"                                                      \
            "mov %%edx, %0\n\t"                                               \
            "mov %%eax, %1\n\t"                                               \
            "cpuid\n\t"                                                       \
            : "=r"(cyc_high), "=r"(cyc_low)::"%rax", "%rbx", "%rcx", "%rdx"); \
        (cycles) = ((uint64_t)cyc_high << 32) | cyc_low;                      \
    } while (0)

#define BEST_TIME(test, repeat, size)                                 \
    do {                                                              \
        printf("%s: ", #test);                                        \
        fflush(NULL);                                                 \
        uint64_t cycles_start, cycles_final, cycles_diff;             \
        uint64_t min_diff = (uint64_t)-1;                             \
		float min_time = 1e+10 * CLOCKS_PER_SEC;                      \
        for (int i = 0; i < repeat; i++) {                            \
			struct timespec tt1, tt2;                                 \
			clock_gettime(CLOCK_REALTIME, &tt1);                      \
            __asm volatile("" ::: /* pretend to clobber */ "memory"); \
            RDTSC_START(cycles_start);                                \
            test;                                                     \
            RDTSC_FINAL(cycles_final);                                \
            cycles_diff = (cycles_final - cycles_start);              \
			clock_gettime(CLOCK_REALTIME, &tt2);                      \
            if (cycles_diff < min_diff) min_diff = cycles_diff;       \
			double diff_time = (tt2.tv_sec + 1e-9*tt2.tv_nsec) -      \
								(tt1.tv_sec + 1e-9*tt1.tv_nsec);      \
			if (diff_time < min_time)                                 \
				min_time = diff_time;                                 \
        }                                                             \
        uint64_t S = (uint64_t)size;                                  \
        float cycle_per_op = (min_diff) / (float)S;                   \
        printf(" %.2f cycles", cycle_per_op);                         \
		printf(" %.3f seconds", min_time);                            \
        printf("\n");                                                 \
        fflush(NULL);                                                 \
	} while (0)

void benchmark_large_serial() {
#define MAXN 5005
	FILE *fin = fopen("test.in", "r");
	assert(fin != NULL && "cannot open file");

	static char A[MAXN], B[MAXN];
	static short GA[MAXN], GB[MAXN];
	int nA, nB;
	while (fscanf(fin, "%s", A) == 1) {
		nA = strlen(A);
		for (int i = 0; i < nA; i++)
			assert(fscanf(fin, "%hd", &GA[i]) == 1);
		assert(fscanf(fin, "%s", B) == 1);
		nB = strlen(B);
		for (int i = 0; i < nB; i++)
			assert(fscanf(fin, "%hd", &GB[i]) == 1);
		BEST_TIME(serial_VGLCS(nA,A,GA,nB,B,GB), 10, 1);
		// printf("%d\n", ret);
	}
#undef MAXN
}

void benchmark_large_parallel() {
#define MAXN 5005
	FILE *fin = fopen("test.in", "r");
	assert(fin != NULL && "cannot open file");

	static char A[MAXN], B[MAXN];
	static short GA[MAXN], GB[MAXN];
	int nA, nB;
	while (fscanf(fin, "%s", A) == 1) {
		nA = strlen(A);
		for (int i = 0; i < nA; i++)
			assert(fscanf(fin, "%hd", &GA[i]) == 1);
		assert(fscanf(fin, "%s", B) == 1);
		nB = strlen(B);
		for (int i = 0; i < nB; i++)
			assert(fscanf(fin, "%hd", &GB[i]) == 1);
		BEST_TIME(parallel_VGLCS(nA,A,GA,nB,B,GB), 10, 1);
		// printf("%d\n", ret);
	}
#undef MAXN
}

int main() {
	benchmark_large_parallel();
	benchmark_large_serial();
	return 0;
}
