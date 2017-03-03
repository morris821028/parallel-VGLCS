#include <bits/stdc++.h>
#include <omp.h>
#include "encrypt.h"

#define likely(x) __builtin_expect(!!(x), 1)
#define unlikely(x) __builtin_expect(!!(x), 0)
#define MIN(x, y) ((x) < (y) ? (x) : (y))
#define MAX(x, y) ((x) > (y) ? (x) : (y))


#define MAXN 131072
static int16_t value[MAXN];
static int32_t x;
static int32_t parent[MAXN], weight[MAXN], left[MAXN];
 
static int findp(int x) {
    return parent[x] == x ? x : (parent[x] = findp(parent[x]));
}
static inline int joint(int l, int r) {
    l = findp(l);
    r = findp(r);
    if (weight[r] > weight[l]) {
        weight[r] += weight[l];
        parent[l] = r, left[r] = left[l];
        return left[r];
    } else {
        weight[l] += weight[r];
        parent[r] = l, value[l] = value[r];
        return left[l];
    }
}
 
void init_ISMQ(int N) {
	#pragma omp for
    for (int i = 0; i < N; i++)
        parent[i] = i;
	#pragma omp for
    for (int i = 0; i < N; i++)
        weight[i] = 1;
	#pragma omp for
    for (int i = 0; i < N; i++)
        left[i] = i;
	#pragma omp single
    x = -1;
}
void append_ISMQ(int16_t V) {
    ++x;
    value[x] = V;
    int y = x-1;
    while (y >= 0) {
        y = findp(y);
        if (value[y] > V)
            return ;
        y = joint(y, x);
        y--;
    }
}
int16_t query_ISMQ(int32_t L) {
    return value[findp(L)];
}

int main() {
	int N, M, S, MOD;
	assert(scanf("%d %d %d %d", &N, &M, &S, &MOD) == 4);

	p_srand(S);

	int32_t L[MAXN] = {};
	int16_t A[MAXN] = {};
	for (int i = 1; i <= N; i++)
		A[i] = p_rand()&MOD;
	for (int i = 1; i <= N; i++)	
		L[i] = i-(p_rand()%MIN(i, MOD));
	int16_t hash  = 0;
#pragma omp parallel
	{
		for (int it = 0; it < M; it++) {
			init_ISMQ(N+1);
			#pragma omp barrier
#pragma omp single
			{
				for (int i = 1; i <= N; i++) {
					append_ISMQ(A[i]);
					assert(L[i] > 0);
					int16_t ret = query_ISMQ(L[i]-1);
					hash ^= ret;
				}
			}
#pragma omp for 
			for (int i = 1; i <= N; i++)
				A[i] = p_func(A[i])&MOD;
		}
	}	
	printf("%X\n", hash);
	return 0;
}
/*
   10 
   3 2 4 5 6 8 1 2 9 7
   7 
   1 5 
   3 5
   1 10
   5 8
   6 6 
   2 4
   2 9 

 */

