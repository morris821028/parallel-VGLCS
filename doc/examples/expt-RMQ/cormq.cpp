#include <bits/stdc++.h>
#include <omp.h>
using namespace std;

static inline int log2int(int x) {
    return __builtin_clz((int) 1) - __builtin_clz(x);
}
#define likely(x) __builtin_expect(!!(x), 1)
#define unlikely(x) __builtin_expect(!!(x), 0)
#define MIN(x, y) ((x) < (y) ? (x) : (y))
#define MAX(x, y) ((x) > (y) ? (x) : (y))
#define LOGS 3
#define POWS (1<<LOGS)
const int MAXN = 30005;
const int MAXLOGN = 20;
struct miniSparseTable {
	int16_t *oA;
    int16_t *tb[MAXLOGN];
    int16_t *pblock, *sblock;
    uint32_t *tree;
    int M, Ms, logMs;
    inline void init(int N, int16_t A[], int8_t *mem) {
		M = ((N+POWS)>>LOGS)<<LOGS;
		Ms = M>>LOGS;
		logMs = log2int(Ms);
		for (int i = 0; i <= logMs; i++)
			tb[i] = (int16_t *) mem, mem += sizeof(int16_t)*(Ms);
		pblock = (int16_t *) mem, mem += sizeof(int16_t)*M;
		sblock = (int16_t *) mem, mem += sizeof(int16_t)*M;
		tree = (uint32_t*) mem, mem += sizeof(uint32_t)*Ms;
    }
    // parallel build
    static uint32_t signature(int16_t A[]) {    	
	    struct Node {
			int16_t val;
			int8_t l, r, p;
		} D[POWS+1];
    	D[0].val = SHRT_MAX;
		D[0].l = D[0].r = D[0].p = 0;
		uint32_t mask = 0;
    	for (int i = 1; i <= POWS; i++) {
    		D[i].val = *A, A++;
    		D[i].l = D[i].r = D[i].p = 0;
    		int8_t p = i-1, cnt = 0;
    		while (D[p].val < D[i].val)
    			p = D[p].p, cnt++;
    		D[D[p].r].p = i;
    		D[i].l = D[p].r, D[i].p = p;
    		D[p].r = i;
    		mask = mask | (cnt<<((i-1)<<2));
		}
    	return mask;
	}
	static inline int8_t RMQ(uint32_t tree, int8_t l, int8_t r) {
		int8_t mxIdx = l, x = 0;
		for (l++; l <= r; l++) {
			x = x + 1 - ((tree>>(l<<2))&15);
			if (x <= 0)
				mxIdx = l, x = 0;
		}
		return mxIdx;
	}
    inline void parallel_build(int16_t A[]) {
    	oA = A;
		int tmplogMs = logMs, tmpMs = Ms;	
		int16_t *tbu = tb[0];
        #pragma omp for schedule(static)
		for (int i = 0; i < M; i += POWS) {
			for (int j = 0, mx = 0; j < POWS; j++) {
				mx = MAX(mx, A[i+j]);
				pblock[i+j] = mx;
			}
			tbu[i>>LOGS] = pblock[i+POWS-1];
		}

		#pragma omp for schedule(static)
		for (int i = 0; i < M; i += POWS) {
			for (int j = POWS-1, mx = 0; j >= 0; j--) {
				mx = MAX(mx, A[i+j]);
				sblock[i+j] = mx;
			}
			tree[i>>LOGS] = signature(A+i);
		}

		for (int k = 1; k <= tmplogMs; k++) {
			int16_t *tbu = tb[k];
            const int16_t *tbv = tb[k-1];
			#pragma omp for schedule(static)
			for (int i = 0; i < tmpMs; i++) {
				if (i-(1<<(k-1)) >= 0) {
                    int16_t p = tbv[i-(1<<(k-1))];
                    int16_t q = tbv[i];
                    tbu[i] = MAX(q, p);
                }
			}
		}
    }
    // dump
    void dump() {
    	printf("pblock[");
    	for (int i = 0; i < M; i++)
    		printf("%2hd%c", pblock[i], " ]"[i==M-1]);
    	puts("");
    	printf("sblock[");
    	for (int i = 0; i < M; i++)
    		printf("%2hd%c", sblock[i], " ]"[i==M-1]);
    	puts("");
    	for (int i = 0; i <= logMs; i++) {
    		printf("tb[%2d][", i);
    		for (int j = 0; j < Ms; j++)
    			printf("%2hd%c", tb[i][j], " ]"[j==Ms-1]);
    		puts("");
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
    inline int16_t get(int l, int r, int t) {
    	if (unlikely((l>>LOGS) == (r>>LOGS)))
    		return oA[((l>>LOGS)<<LOGS)+RMQ(tree[l>>LOGS], l&7, r&7)];
		int16_t ret = 0;
		if ((r&7) != 7) {
			ret = MAX(pblock[r], ret);
			r = r-1-(r&7);
		}
		if (l&7) {
			ret = MAX(sblock[l], ret);
			l = l+8-(l&7);
		}
		if (unlikely(l > r))
			return ret;
		l = l>>LOGS;
		r = r>>LOGS;
        int16_t p = tb[t][l+(1<<t)-1];
        int16_t q = tb[t][r];
        ret = MAX(p, ret);
		ret = MAX(q, ret);
        return ret;
    }
    static int f(int N) {
    	const int M = ((N+POWS)>>LOGS)<<LOGS;
    	const int Ms = M>>LOGS;
    	const int logMs = log2int(Ms);
    	return sizeof(int16_t)*(logMs+1)*(Ms)
			+ sizeof(int16_t)*M*2
			+ sizeof(uint32_t)*Ms;
	}
};
//#undef LOGS
//#undef POWS

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
	for (int i = 1; i <= N; i++)
		l[i] = p_random()%min(i, 100)+1;
	int8_t logG[MAXN];
	for (int i = 1; i <= N; i++) {
		int ql = l[i], qr = i;
		if (ql&7) {
			ql = ql+(8-(ql&7));
		}
		if ((qr&7) != 7) {
			qr = qr-(qr&7)-1;
		}
		ql = ql>>LOGS;
		qr = qr>>LOGS;
		logG[i] = log2int(qr-ql+1);
	}
	miniSparseTable sp_tlb;
	{
		int8_t *mem = (int8_t*) malloc(miniSparseTable::f(N));
		sp_tlb.init(N, A, mem);
	}
	const int P = 20;
	omp_set_num_threads(P);
	int16_t hash  = 0;
	#pragma omp parallel
	{
		for (int it = 0; it < 30001; it++) {

			sp_tlb.parallel_build(A);
			
			#pragma omp for reduction(^: hash)
			for (int i = 1; i <= N; i++) {
				int L = l[i], R = i;
				if (L > R)	swap(L, R);
				int16_t ret = sp_tlb.get(L, R, logG[i]);
				hash ^= ret;
			}
		}
	}	
	printf("%hd\n", hash);
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
