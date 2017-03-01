#include "ISMQ.h"
 
#define MAXN (1<<24)
static int32_t lIdx, x;
static uint32_t value[MAXN], parent[MAXN];
static uint32_t weight[MAXN], leader[MAXN];
static uint32_t findp(uint32_t x) {
    return parent[x] == x ? x : (parent[x] = findp(parent[x]));
}
void init_ISMQ(int N) {
    x = -1, lIdx = -1;
}
void append_ISMQ(uint32_t V) {
    ++x;
    parent[x] = x;
    uint32_t u = x, weightR = 1;
    for (uint32_t *v = leader + lIdx; lIdx >= 0 && value[*v] <= V; v--, lIdx--) {
        if (weightR <= weight[lIdx])
            u = (parent[u] = *v);
        else
            parent[*v] = u;
        weightR = weight[lIdx] + weightR;
    }
    ++lIdx;
    value[u] = V;
    leader[lIdx] = u;
    weight[lIdx] = weightR; 
}
uint32_t query_ISMQ(uint32_t L) {
    return value[findp(L)];
}
