#include "ISMQ.h"
#include <algorithm>
#include <string.h>
using namespace std;

#define MAXN 16777216
static uint32_t value[MAXN];
static int32_t parent[MAXN];
static int32_t weight[MAXN];
static uint32_t leader[MAXN][2];
static int lIdx, x;
static int findp(int x) {
	return parent[x] == x ? x : (parent[x] = findp(parent[x]));
}
static inline int joint(int l, int r, uint32_t V) {
	if (weight[r] >= weight[l]) {
		weight[r] += weight[l];
		parent[l] = r;
		return r;
	} else {
		weight[l] += weight[r];
		parent[r] = l;
		value[l] = V;
		return l;
	}
}

void init_ISMQ(int N) {
	x = -1, lIdx = -1;
}
void append_ISMQ(uint32_t V) {
	++x;
	value[x] = V, parent[x] = x, weight[x] = 1;
	int u = x;
	for (; lIdx >= 0 && leader[lIdx][1] <= V; lIdx--)
		u = joint(leader[lIdx][0], u, V);
	++lIdx;
	leader[lIdx][0] = u;
	leader[lIdx][1] = V;
}
uint32_t query_ISMQ(uint32_t L) {
	return value[findp(L)];
}

