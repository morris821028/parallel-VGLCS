#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <algorithm>
#include <omp.h>
#include "VGLCS.h"

const int MAXN = 10005;
const int MAXLOGN = 16;
struct ISMQ {
	uint16_t *value;
	uint16_t *parent, *left;
	int findp(int x) {
		int u = x;
		while (u != parent[u])
			u = parent[u];
		while (x != parent[x]) {
			uint16_t tmp = parent[x];
			parent[x] = u;
			x = tmp;
		}
		return u;
	}
	inline int joint(int l, int r) {
		l = findp(l);
		r = findp(r);
		parent[l] = r, left[r] = left[l];
		return left[l];
	}
	void init(int n, uint16_t *mem_base) {
		parent = mem_base;
		left = mem_base+n+1;
		value = mem_base+2*n+2;
		for (int i = 0; i <= n; i++)
			parent[i] = i;
		for (int i = 0; i <= n; i++)
			left[i] = i;
		memset(value, 0, sizeof(value[0])*(n+1));
	}
	int get(int x) {
		return value[findp(x)];
	}
	void set(int x, int val) {
		value[x] = val;
		int y = x-1;
		while (y >= 0) {
			y = findp(y);
			if (value[y] > val)
				return ;
			y = joint(y, x);
			y--;
		}
	}
};
static inline int log2int(int x) {
    return __builtin_clz((int)1) - __builtin_clz(x);
}

#define MIN(x, y) ((x) < (y) ? (x) : (y))
#define MAX(x, y) ((x) > (y) ? (x) : (y))
int serial_VGLCS(int nA, char A[], uint16_t GA[],
        int nB, char B[], uint16_t GB[]) {
    A--, B--, GA--, GB--;
	assert(nA < MAXN);

	uint16_t *mem_base = (uint16_t *) malloc(sizeof(uint16_t)*(nA+1)*(nB+1)*3);
	uint16_t *mem_ismq = (uint16_t *) malloc(sizeof(uint16_t)*(nB+1)*3);
	assert(mem_base != NULL);
	assert(mem_ismq != NULL);
    ISMQ Q[MAXN];
    for (int i = 0; i <= nB; i++)
        Q[i].init(nA, mem_base+(i*(nA+1)*3));

    uint16_t ret = 0;
    for (int i = 1; i <= nA; i++) {
        int p_begin = i - MIN(GA[i]+1, i);
        ISMQ RQ;
        RQ.init(nB, mem_ismq);
        for (int j = 1; j <= nB; j++) {
			uint16_t tmp = 0;
            if (A[i] == B[j]) {
                tmp = RQ.get(j - MIN(GB[j]+1, j))+1;
            }
            RQ.set(j, Q[j].get(p_begin));
            Q[j].set(i, tmp);
            ret = MAX(ret, tmp);
        }
    }
	free(mem_base);
	free(mem_ismq);
    return ret;
}

int parallel_VGLCS(int nA, char A[], uint16_t GA[], 
					int nB, char B[], uint16_t GB[]) {
	A--, B--, GA--, GB--;
	if (nA > nB) {
		std::swap(GA, GB), std::swap(nA, nB), std::swap(A, B);
	}
	assert(nA < MAXN && nB < MAXN);
	uint16_t *mem_base = (uint16_t *) malloc(sizeof(uint16_t)*(nA+1)*(nB+1)*3);
	assert(mem_base != NULL);
	ISMQ Q[MAXN];

	uint16_t ret = 0, max_gap = 0;
	for (int i = 1; i <= nB; i++)
		max_gap = MAX(max_gap, GB[i]);
	max_gap = MIN(max_gap, nB);

	const int lognB = log2int(max_gap+1);
	uint16_t *mem_tlb = (uint16_t *) malloc(sizeof(uint16_t)*(nB+1)*(lognB+1));
	uint16_t *tb[MAXLOGN] = {};
	for (int i = 0; i < MAXLOGN; i++)
		tb[i] = mem_tlb + (i * (nB+1));

	const int P = 20;
	omp_set_num_threads(P);
	const int chunk = MAX((nB+P-1) / P, 16);
	
	char logGB[MAXN];
	for (int i = 1; i <= nB; i++) {
		int l = i - MIN(GB[i]+1, i), r = i-1;
		logGB[i] = log2int(r-l+1);
	}

#ifdef _LIMIT_DA
	char limGB[MAXN] = {};
	for (int i = nB; i >= 1; i--) {
		limGB[i] = MAX(logGB[i], limGB[i]);
		for (int k = 1; k <= limGB[i]; k++) {
			int p = MAX(i-(1<<(k-1)), 0);
			limGB[p] = MAX(limGB[p], k-1);
		}
		int l = i - MIN(GB[i]+1, i);
		int t = logGB[i];
		limGB[l+(1<<t)-1] = MAX(limGB[l+(1<<t)-1], t);
	}
#endif

#if defined(_REPORT) and defined(_LIMIT_DA)
	int reduce = 0;
	for (int i = 1; i <= nB; i++)
		reduce += lognB - limGB[i];
	printf("target reduces %d\n", reduce);
#endif

	#pragma omp parallel
	{
		// init ISMQ
		#pragma omp for schedule(static, chunk)
		for (int i = 0; i <= nB; i++)
			Q[i].init(nA, mem_base+(i*(nA+1)*3));

		for (int i = 1; i <= nA; i++) {
			int p_begin = i - MIN(GA[i]+1, i);
			// query: incremental suffix maximum query
			#pragma omp for schedule(static, chunk)
			for (int j = 1; j <= nB; j++)
				tb[0][j] = Q[j].get(p_begin);
				
			// doubling algorithm
			for (int k = 1; k <= lognB; k++) {
				uint16_t *tbu = tb[k];
				const uint16_t *tbv = tb[k-1];
				#pragma omp for schedule(static, chunk)
				for (int j = 1; j <= nB; j++) {
#ifdef _LIMIT_DA
					if (j-(1<<(k-1)) >= 0 && k <= limGB[j]) {
#else					
					if (j-(1<<(k-1)) >= 0) {
#endif
						const uint16_t p = tbv[j-(1<<(k-1))];
						const uint16_t q = tbv[j];
						tbu[j] = MAX(q, p);
					}
				}
			}

			char Ai = A[i];
			// dynamic programming
			#pragma omp for schedule(static, chunk) reduction(max: ret)
			for (int j = 1; j <= nB; j++) {
				if (Ai == B[j]) {
					int l = j - MIN(GB[j]+1, j), r = j-1;
					int t = logGB[j];
					const uint16_t p = tb[t][l+(1<<t)-1];
					const uint16_t q = tb[t][r];
					uint16_t val = MAX(q, p)+1;
					Q[j].set(i, val);
					ret = MAX(ret, val);
				}
			}
		}
	}
	free(mem_base);
	free(mem_tlb);
	return ret;
}
#undef MIN
#undef MAX
