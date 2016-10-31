#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "VGLCS.h"

void test_small() {
	char A[] = "RCLPCRR";
	short GA[] = {2, 3, 0, 0, 3, 2, 2};
	char B[] = "RPPLCPLRC";
	short GB[] = {2, 0, 0, 0, 3, 0, 0, 2, 3};
	int nA = strlen(A);
	int nB = strlen(B);
	assert(serial_VGLCS(nA, A, GA, nB, B, GB) == 3);
	assert(parallel_VGLCS(nA, A, GA, nB, B, GB) == 3);
}

int main() {
	test_small();
	printf("All asserts passed. Code is probably ok.\n");
	return 0;
}

