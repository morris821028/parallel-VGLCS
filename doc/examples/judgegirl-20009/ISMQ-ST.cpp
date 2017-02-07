#include "ISMQ.h"
#include <algorithm>
using namespace std;
#define MAXLOGN 24
#define MAXN 16777216

static uint32_t mem[MAXN*MAXLOGN];
static uint32_t *tb[MAXLOGN];
static int x = 0;

static inline int log2int(int x) {
	return 31 - __builtin_clz(x); 
}

void init_ISMQ(int N) {
	int logN = log2int(N);
	for (int i = 0; i <= logN; i++)
		tb[i] = mem + i*N;
	x = -1;
}
void append_ISMQ(uint32_t V) {
	++x;
	tb[0][x] = V;
	int limG = log2int(x+1);
	for (int i = 1; i <= limG; i++) {
		uint32_t p = tb[i-1][x-(1<<(i-1))];
		uint32_t q = tb[i-1][x];
		tb[i][x] = max(q, p);
	}
}
uint32_t query_ISMQ(uint32_t L) {
	uint32_t R = x;
	int t = log2int(R-L+1);
	uint32_t p = tb[t][L+(1<<t)-1] ;
	uint32_t q = tb[t][R];
	return max(q, p);
}
