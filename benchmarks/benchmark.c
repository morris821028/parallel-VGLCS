#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <assert.h>
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



#define BEST_TIME(test,  repeat, size)                         \
    do {                                                              \
        printf("%s: ", #test);                                        \
        fflush(NULL);                                                 \
        uint64_t cycles_start, cycles_final, cycles_diff;             \
        uint64_t min_diff = (uint64_t)-1;                             \
        for (int i = 0; i < repeat; i++) {                            \
            __asm volatile("" ::: /* pretend to clobber */ "memory"); \
            RDTSC_START(cycles_start);                                \
            test;                                                     \
            RDTSC_FINAL(cycles_final);                                \
            cycles_diff = (cycles_final - cycles_start);              \
            if (cycles_diff < min_diff) min_diff = cycles_diff;       \
        }                                                             \
        uint64_t S = (uint64_t)size;                                  \
        float cycle_per_op = (min_diff) / (float)S;                   \
        printf(" %.2f cycles per operation", cycle_per_op);           \
        printf("\n");                                                 \
        fflush(NULL);                                                 \
} while (0)

void benchmark_large_serial() {
#define MAXN 5005
	FILE *fin = fopen("5.in", "r");
	FILE *fout = fopen("5.out", "r");
	assert(fin != NULL && "cannot open file 5.in");
	assert(fout != NULL && "cannot open file 5.out");

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
		int ret = parallel_VGLCS(nA, A, GA, nB, B, GB);
		int ac_ret;
		assert(fscanf(fout, "%d", &ac_ret) == 1);
		assert(ac_ret == ret && "failed compare output");
		BEST_TIME(serial_VGLCS(nA, A, GA, nB, B, GB), 10, 1);
		// printf("%d\n", ret);
	}
#undef MAXN
}

void benchmark_large_parallel() {
#define MAXN 5005
	FILE *fin = fopen("5.in", "r");
	FILE *fout = fopen("5.out", "r");
	assert(fin != NULL && "cannot open file 5.in");
	assert(fout != NULL && "cannot open file 5.out");

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
		int ret = parallel_VGLCS(nA, A, GA, nB, B, GB);
		int ac_ret;
		assert(fscanf(fout, "%d", &ac_ret) == 1);
		assert(ac_ret == ret && "failed compare output");
		BEST_TIME(parallel_VGLCS(nA, A, GA, nB, B, GB), 10, 1);
		// printf("%d\n", ret);
	}
#undef MAXN
}

int main() {
	benchmark_large_parallel();
	benchmark_large_serial();
	return 0;
}
