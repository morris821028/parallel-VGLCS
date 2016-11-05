#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <assert.h>
#include "VGLCS.h"

void test_same() {
	static char A[1024];
	static char B[1024];
	uint16_t GA[1024];
	uint16_t GB[1024];
	int n = 1000, m = 500;
	for (int i = 0; i < n; i++)
		A[i] = 'M';
	for (int i = 0; i < m; i++)
		B[i] = 'M';
	A[n] = '\0', B[m] = '\0';
	for (int i = 0; i < n; i++)
		GA[i] = rand();
	for (int i = 0; i < m; i++)
		GB[i] = rand();
	int expected = n < m ? n : m;
	assert(serial_VGLCS(n, A, GA, m, B, GB) == expected);
	assert(parallel_VGLCS(n, A, GA, m, B, GB) == expected);
}

void test_small() {
	char A[] = "RCLPCRR";
	uint16_t GA[] = {2, 3, 0, 0, 3, 2, 2};
	char B[] = "RPPLCPLRC";
	uint16_t GB[] = {2, 0, 0, 0, 3, 0, 0, 2, 3};
	int nA = strlen(A);
	int nB = strlen(B);
	assert(serial_VGLCS(nA, A, GA, nB, B, GB) == 3);
	assert(parallel_VGLCS(nA, A, GA, nB, B, GB) == 3);
}

int main() {
	test_small();
	test_same();
	printf("All asserts passed. Code is probably ok.\n");
	return 0;
}

