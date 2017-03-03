#include <bits/stdc++.h>
#include <omp.h>
using namespace std;
 
static inline int log2int(int x) {
    return 31 - __builtin_clz(x);
}
 
struct ISMQ {
    int16_t *value;
    int16_t *parent;
	int16_t *weight;
	int16_t *leader;
	int16_t lIdx;
    int findp(int x) {
        return parent[x] == x ? x : (parent[x] = findp(parent[x]));
    }
	void init(int n, int16_t *mem_base) {
		parent = mem_base;
		value = mem_base+(n+1)*1;
		weight = mem_base+(n+1)*2;
/*
		int16_t *tmp;
        tmp = parent;
        for (int i = 0; i <= n; i++)
            *tmp = i, tmp++;
        tmp = weight;
        for (int i = 0; i <= n; i++)
            *tmp = 1, tmp++;
        memset(value, 0, sizeof(int16_t)*(n+1));
*/
		leader = mem_base+(n+1)*3;
		lIdx = 0;
		value[0] = 0, parent[0] = 0, weight[0] = 1;
		leader[lIdx] = 0;
		
	}
	int get(int x) {
		return value[findp(x)];
	}
	void append(int x, int16_t val) {
		parent[x] = x;
		int u = x, weightR = 1;
		for (int16_t *v = leader + lIdx; lIdx >= 0 && value[*v] <= val; v--, lIdx--) {
			if (weightR <= weight[lIdx])
				u = (parent[u] = *v);
			else
				parent[*v] = u;
			weightR = weight[lIdx] + weightR;
		}
		++lIdx;
		value[u] = val;
		leader[lIdx] = u;
		weight[lIdx] = weightR; 
	}
};
#define MIN(x, y) ((x) < (y) ? (x) : (y))
#define MAX(x, y) ((x) > (y) ? (x) : (y))
const int MAXN = 10005;
const int MAXLOGN = 15;
struct SparseTable {
	int16_t *tb[MAXLOGN];
	// create tb[0..logN][0..n]
	inline void init(int n, int logN, int16_t *mem_base) {
		for (int i = 0; i <= logN; i++) {
			tb[i] = mem_base + i*(n+1);
			tb[i][0] = 0;
		}
	}
	// parallel build
	inline void parallel_build(int n, int logN) {
		for (int k = 1; k <= logN; k++) {
			int16_t *tbu = tb[k];
			const int16_t *tbv = tb[k-1];
#pragma omp for schedule(static)
			for (int i = 1; i <= n; i++) {
				if (i-(1<<(k-1)) >= 0) {
					int16_t p = tbv[i-(1<<(k-1))];
					int16_t q = tbv[i];
					tbu[i] = MAX(q, p);
				}
			}
		}
	}
	// set tb[0][x] = val, and update its relationship
	inline void set(int x, int16_t val, int limG) {
		tb[0][x] = val;
		for (int i = 1; i <= limG && (1<<(i-1)) <= x; i++) {
			int16_t p = tb[i-1][x-(1<<(i-1))];
			int16_t q = tb[i-1][x];
			tb[i][x] = MAX(q, p);
		}
	}
	// query the maximum value of interval [l..r]
	inline int16_t get(int l, int r) {
		int t = log2int(r-l+1);
		int16_t p = tb[t][l+(1<<t)-1];
		int16_t q = tb[t][r];
		return MAX(q, p);
	}
	inline int16_t get(int l, int r, int t) {
		int16_t p = tb[t][l+(1<<t)-1];
		int16_t q = tb[t][r];
		return MAX(q, p);
	}
};
int parallel_VGLCS(int nA, char A[], int16_t GA[],
		int nB, char B[], int16_t GB[]) {
	A--, B--, GA--, GB--;
	if (nA > nB) {
		std::swap(GA, GB), std::swap(nA, nB), std::swap(A, B);
	}
	assert(nA < MAXN && nB < MAXN);

	ISMQ Q[MAXN];
	char logGB[MAXN];
	int16_t ret = 0, max_gap = 0;

	{
		for (int i = 1; i <= nB; i++)
			max_gap = MAX(max_gap, GB[i]);

		max_gap = MIN(max_gap, nB);

		for (int i = 1; i <= nB; i++) {
			int l = i - MIN(GB[i]+1, i), r = i-1;
			logGB[i] = log2int(r-l+1);
		}
	}

	const int P = 20;
	const int lognB = log2int(max_gap+1);
	int16_t *mem_tlbD = (int16_t *) malloc(sizeof(int16_t)*(nA+1)*4*(nB+1));
	int16_t *mem_tlbR = (int16_t *) calloc((nB+1)*(lognB+1), sizeof(int16_t));   
	SparseTable sp_tlb;

	{
		assert(mem_tlbD != NULL);
		assert(mem_tlbR != NULL);
		sp_tlb.init(nB, lognB, mem_tlbR);
		omp_set_num_threads(P);
	}

#pragma omp parallel
	{   
		// init ISMQ
#pragma omp for schedule(static)
		for (int i = 0; i <= nB; i++)
			Q[i].init(nA, mem_tlbD+(i*(nA+1)*4));

		for (int i = 1; i <= nA; i++) {
			// query: incremental suffix maximum query
			{
				int r = i - MIN(GA[i]+1, i);
				int16_t *tb = sp_tlb.tb[0];
#pragma omp for schedule(static)
				for (int j = 1; j <= nB; j++)
					tb[j] = Q[j].get(r);
			}

			// doubling algorithm
			sp_tlb.parallel_build(nB, lognB);

			char Ai = A[i];
			// dynamic programming
#pragma omp for schedule(static) reduction(max: ret)
			for (int j = 1; j <= nB; j++) {
				if (Ai == B[j]) {
					int l = j - MIN(GB[j]+1, j), r = j-1;
					int16_t val = sp_tlb.get(l, r, logGB[j])+1;
					Q[j].append(i, val);
					ret = MAX(ret, val);
				} else {
					Q[j].append(i, 0);
				}
			}
		}
	}
	free(mem_tlbD);
	free(mem_tlbR);
	return ret;
}

int main() {
	static char A[MAXN], B[MAXN];
	static int16_t GA[MAXN], GB[MAXN];
	int nA, nB, Q;
	while (scanf("%s", A) == 1) {
		nA = strlen(A);
		for (int i = 0; i < nA; i++)
			scanf("%hd", &GA[i]);
		scanf("%s", B);
		nB = strlen(B);
		for (int i = 0; i < nB; i++)
			scanf("%hd", &GB[i]);
		int ret = parallel_VGLCS(nA, A, GA, nB, B, GB);
		printf("%d\n", ret);
	}
	return 0;
}





