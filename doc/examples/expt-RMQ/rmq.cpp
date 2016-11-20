#include <bits/stdc++.h>
#include <omp.h>
using namespace std;

static inline int log2int(int x) {
    return 31 - __builtin_clz(x);
}

#define MIN(x, y) ((x) < (y) ? (x) : (y))
#define MAX(x, y) ((x) > (y) ? (x) : (y))
const int MAXN = 30005;
const int MAXLOGN = 20;
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

int16_t A[MAXN] = {};
unsigned int seed = 0;
unsigned int p_random() {return seed = (seed*9301+49297);}

int main() {
	freopen("2.in", "r", stdin);
	int N, M, S;
	scanf("%d %d %d", &N, &M, &S);
	seed = S;

	for (int i = 1; i <= N; i++)
		A[i] = p_random()%N;
	
	int l[MAXN];
	int8_t logG[MAXN];
	for (int i = 1; i <= N; i++) {
		l[i] = p_random()%min(i, 100)+1;
		logG[i] = log2int(i-l[i]+1);
	}

	const int logN = log2int(N+1);
	SparseTable sp_tlb;	
	{
		int16_t *mem = (int16_t *) malloc(sizeof(int16_t)*(logN+1)*(N+1));		
		sp_tlb.init(N, logN, mem);
		for (int i = 0; i <= N; i++)
			sp_tlb.tb[0][i] = A[i];
	}

	const int P = 20;
	omp_set_num_threads(P);
	int16_t hash  = 0;
	#pragma omp parallel
	{
		for (int it = 0; it < 30001; it++) {

			sp_tlb.parallel_build(N, logN);
			
			#pragma omp for reduction(^: hash)
			for (int i = 1; i <= N; i++) {
				int L = l[i], R = i;
				if (L > R)	swap(L, R);
				int16_t ret = sp_tlb.get(L, R, logG[i]);
				hash ^= ret;
			}
		}
	}	
	printf("%d\n", hash);

	return 0;
}

