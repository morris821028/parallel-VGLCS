#include <bits/stdc++.h>
#include <omp.h>
using namespace std;

static inline int log2int(int x) {
	return 31 - __builtin_clz(x); 
}

struct ISMQ {
	int16_t *value;
	int16_t *parent, *left;
	void init(int n, int16_t *mem_base) {
		parent = mem_base;
		value = mem_base+n+1;
		left = mem_base+(n+1)*2;
		int16_t *tmp;
		tmp = parent;
		for (int i = 0; i <= n; i++)
			*tmp = i, tmp++;
		memset(value, 0, sizeof(int16_t)*(n+1));
		tmp = left;
		for (int i = 0; i <= n; i++)
			*tmp = i, tmp++;
	}

	int findp(int x) {
        return parent[x] == x ? x : (parent[x] = findp(parent[x]));
    }
	inline int joint(int l, int r) {
		l = findp(l);
		r = findp(r);
		parent[l] = r;
		return left[r] = left[l];
	}
	inline int get(int x) {
		return value[findp(x)];
	}
	inline void set(int x, int val) {
		value[x] = val;
		int y = x-1;
		while (y >= 0) {
			y = findp(y);
			if (value[y] > val)
				return ;
			x = y = joint(y, x);
			y--;
		}
	}
};
#define MIN(x, y) ((x) < (y) ? (x) : (y))
#define MAX(x, y) ((x) > (y) ? (x) : (y))
const int MAXN = 5005;
const int MAXLOGN = 13;
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

	const int P = 10;
	const int lognB = log2int(max_gap+1);
	int16_t *mem_tlbD = (int16_t *) malloc(sizeof(int16_t)*(nA+1)*3*(nB+1));
	int16_t *mem_tlbR = (int16_t *) calloc((nB+1)*(lognB+1), sizeof(int16_t));	
	SparseTable sp_tlb;
	
	{
		assert(mem_tlbD != NULL);
		assert(mem_tlbR != NULL);
		sp_tlb.init(nB, lognB, mem_tlbR);
		omp_set_num_threads(P);
	}
	
	static int16_t next_pos[128][MAXN] = {};
	#pragma omp parallel for
	for (int i = 'A'; i <= 'Z'; i++) {
		int16_t *pos = next_pos[i];
		pos[nB+1] = nB+1;
		for (int j = nB; j >= 1; j--) {
			if (B[j] != i) {
				pos[j] = pos[j+1];
			} else {
				pos[j] = j;
			}
		}
	}
	
	#pragma omp parallel
	{	
		// init ISMQ
		#pragma omp for schedule(static)
		for (int i = 0; i <= nB; i++)
			Q[i].init(nA, mem_tlbD+(i*(nA+1)*3));

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
				if (Ai != B[j]) {
					j = next_pos[Ai][j]-1;
				} else {
					int l = j - MIN(GB[j]+1, j), r = j-1;
					int16_t val = sp_tlb.get(l, r, logGB[j])+1;
					Q[j].set(i, val);
					ret = MAX(ret, val);
				}
			}
		}
	}
	free(mem_tlbD);
	free(mem_tlbR);
	return ret;
}


int main() {
	// $ icpc -std=c++11 -O2 -qopenmp main.cpp -o main
	// kmp_set_defaults("KMP_AFFINITY=granularity=fine,compact");
	static char A[MAXN], B[MAXN];
	static int16_t GA[MAXN], GB[MAXN];
	int nA, nB, Q;
	while (scanf("%s %s", A, B) == 2) {
		nA = strlen(A);
		nB = strlen(B);
		scanf("%d", &Q);

		for (int it = 0; it < Q; it++) {
			int GLCS = 0, gap[128];
			char s[MAXN];
			for (int i = 0; i < 128; i++)
				gap[i] = 0x3f3f;
			while (scanf("%s", s) == 1 && s[0] != '$')
				scanf("%d", &gap[s[0]]);
			for (int i = 0; i < nA; i++)
				GA[i] = gap[A[i]];
			for (int i = 0; i < nB; i++)
				GB[i] = gap[B[i]];
			int ret = parallel_VGLCS(nA, A, GA, nB, B, GB);
			printf("%d%c", ret, it == Q-1 ? '\n' : ' ');
		}
	}
	return 0;
}

