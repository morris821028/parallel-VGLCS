#include "ISMQ.h"
#include <algorithm>
#include <string.h>
using namespace std;

#define MAXN 16777216
static int Mindex, N;
static uint32_t A[MAXN+1];

static void modify(int x, uint32_t val) {
	for (; x; x -= (x&(-x))) {
		A[x] = max(A[x], val);
	}
}
static uint32_t query(int x) {
	uint32_t ret = 0;
	for (; x <= N; x += (x&(-x))) {
		ret = max(ret, A[x]);
	}
	return ret;
}
void init_ISMQ(int n) {
	Mindex = -1, N = n+1;
	memset(A, 0, sizeof(A[0])*N);
}
void append_ISMQ(uint32_t V) {
	Mindex++;
	modify(Mindex+1, V);
}
uint32_t query_ISMQ(uint32_t L) {
	return query(L+1);
}

