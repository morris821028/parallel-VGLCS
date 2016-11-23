#include <bits/stdc++.h>
#include <omp.h>
using namespace std;

static inline int log2int(int x) {
    return 31 - __builtin_clz(x);
}

#define MIN(x, y) ((x) < (y) ? (x) : (y))
#define MAX(x, y) ((x) > (y) ? (x) : (y))
const int MAXN = 100005;
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

uint32_t seed = 0;
uint32_t p_func(uint32_t x) {return x*9301+49297;}
uint32_t p_random() {return seed = p_func(seed);}

int main() {
	int N, M, S, MOD;
	scanf("%d %d %d %d", &N, &M, &S, &MOD);

	seed = S;

	int16_t A[MAXN] = {};
	int32_t L[MAXN] = {};
	for (int i = 1; i <= N; i++)
		A[i] = p_random()&MOD;
	for (int i = 1; i <= N; i++) {
		L[i] = i-(p_random()%min(i, MOD));
		assert(L[i] <= i);
		assert(L[i] >= 0);
	}

	int8_t logG[MAXN];
	int8_t logN = 0;
	for (int i = 1; i <= N; i++) {
		logG[i] = log2int(i-L[i]+1);
		logN = MAX(logN, logG[i]);
	}
	logN = MIN(logN, log2int(N+1));
	SparseTable sp_tlb;	
	{
		int16_t *mem = (int16_t *) malloc(sizeof(int16_t)*(logN+1)*(N+1));	
		fprintf(stderr, "Compressed %zu\n", sizeof(int16_t)*(logN+1)*(N+1));
		sp_tlb.init(N, logN, mem);
	}

	const int P = 20;
	omp_set_num_threads(P);
	uint16_t hash  = 0;
	#pragma omp parallel
	{
		for (int it = 0; it < M; it++) {
			#pragma omp for
			for (int i = 0; i <= N; i++)
				sp_tlb.tb[0][i] = A[i];

			sp_tlb.parallel_build(N, logN);
			
			#pragma omp for reduction(^: hash)
			for (int i = 1; i <= N; i++) {
				int l = L[i], r = i;
				int16_t ret = sp_tlb.get(l, r, logG[i]);
				hash ^= ret;
			}

			#pragma omp for 
			for (int i = 1; i <= N; i++)
				A[i] = p_func(A[i])&MOD;
		}
	}	
	printf("%X\n", hash);

	return 0;
}

