#include "ISMQ.h"
 
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
    value[x] = V, parent[x] = x;
    int u = x, weightR = 1;
    for (int32_t *v = leader + lIdx; lIdx >= 0 && value[*v] <= V; v--, lIdx--) {
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
