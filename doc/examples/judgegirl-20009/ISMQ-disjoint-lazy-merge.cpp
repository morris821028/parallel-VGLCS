#include "ISMQ.h"
#include <stdio.h> 
#define MAXN (1<<24)
int lIdx, x;
uint32_t value[MAXN], parent[MAXN];
int32_t weight[MAXN], leader[MAXN];
static int32_t findp(int32_t x) {
    return parent[x] == x ? x : (parent[x] = findp(parent[x]));
}
void init_ISMQ(int N) {
    x = -1, lIdx = -1;
}
void append_ISMQ(uint32_t V) {
    ++x;
	int32_t maxW = 1, u = x, sum = 1;
	int rIdx = lIdx;
	for (int32_t *v = leader + lIdx; lIdx >= 0 && value[*v] <= V; v--, lIdx--) {
		if (weight[lIdx] > maxW)
			sum = maxW = weight[lIdx], u = *v;
		else if (weight[lIdx] == maxW)
			sum ++;
	}
	for (int32_t *v = leader + lIdx+1, *vend = leader + rIdx+1; v != vend; v++)
		parent[*v] = u;
	parent[x] = u;
	++lIdx;
	value[u] = V;
	leader[lIdx] = u;
	weight[lIdx] = sum; 
}
uint32_t query_ISMQ(uint32_t L) {
	return value[findp(L)];
}
