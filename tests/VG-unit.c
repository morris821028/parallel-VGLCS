#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <assert.h>
#include "VGLCS.h"

int min(int x, int y) {
	return x < y ? x : y;
}
int max(int x, int y) {
	return x > y ? x : y;
}
int trivial_VG(int nA, char A[], uint16_t GA[],
		int nB, char B[], uint16_t GB[]) {
	assert(nA <= 1000 && nB <= 1000);
#define MAXN 1024
	A--, B--, GA--, GB--;
	static uint16_t dp[MAXN][MAXN];
	memset(dp, 0, sizeof(dp));

	uint16_t ret = 0;
	for (int i = 1; i <= nA; i++) {
		for (int j = 1; j <= nB; j++) {
			if (A[i] == B[j]) {
				int ga = GA[i], gb = GB[j];
				uint16_t mx = 0;
				for (int p = min(ga+1, i); p >= 1; p--)
					for (int q = min(gb+1, j); q >= 1; q--)
						mx = max(mx, dp[i-p][j-q]);
				mx++;
				dp[i][j] = mx;
				ret = max(ret, mx);
			} else {
				dp[i][j] = 0;
			}
//			printf("%d ", dp[i][j]);
		}
//		puts("");
	}
#undef MAXN
	return ret;
}
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
	assert(trivial_VG(nA, A, GA, nB, B, GB) == 3);
	assert(serial_VGLCS(nA, A, GA, nB, B, GB) == 3);
	assert(parallel_VGLCS(nA, A, GA, nB, B, GB) == 3);
}

int main() {
	test_small();
	test_same();
	printf("All asserts passed. Code is probably ok.\n");
	return 0;
}

