#include <bits/stdc++.h>
#include <omp.h>
using namespace std;

static inline int log2int(int x) {
	return 31 - __builtin_clz(x);
}
#define likely(x) __builtin_expect(!!(x), 1)
#define unlikely(x) __builtin_expect(!!(x), 0)
#define MIN(x, y) ((x) < (y) ? (x) : (y))
#define MAX(x, y) ((x) > (y) ? (x) : (y))
#define LOGS 4
#define POWS (1<<LOGS)
#define GET(mark, x) (mark[(x)>>5]>>((x)&31)&1)
#define SET(mark, x) (mark[(x)>>5] |= 1<<((x)&31))
const int MAXN = 100005;
const int MAXLOGN = 20;
struct miniSparseTable {
	int16_t *oA;
	int16_t *tb[MAXLOGN];
	int16_t *pblock, *sblock;
	uint64_t *tree;
	int M, Ms, logMs;
	inline void init(int N, int logN, int8_t *mem) {
		M = ((N+POWS)>>LOGS)<<LOGS;
		Ms = M>>LOGS;
		logMs = MIN(log2int(Ms), logN);
		for (int i = 0; i <= logMs; i++)
			tb[i] = (int16_t *) mem, mem += sizeof(int16_t)*(Ms);
		pblock = (int16_t *) mem, mem += sizeof(int16_t)*M;
		sblock = (int16_t *) mem, mem += sizeof(int16_t)*M;
		tree = (uint64_t*) mem, mem += sizeof(uint64_t)*Ms;
	}
	// parallel build
	static uint64_t signature(int16_t A[]) {    	
		int16_t D[POWS+1];
		D[0] = SHRT_MAX;
		uint64_t mask = 0;
		for (int i = 1, Dp = 0; i <= POWS; i++) {
			int16_t v = *A;
			A++;
			int8_t cnt = 0;
			while (D[Dp] < v)
				Dp--, cnt++;
			D[++Dp] = v;
			mask = mask | (((uint64_t) cnt)<<((i-1)<<2));
		}
		return mask;
	}
	static inline int8_t RMQ(uint64_t tree, int8_t l, int8_t r) {
		int8_t mxIdx = l, x = 0;
		assert(l <= r);
		for (l++; l <= r; l++) {
			x = x+1 - ((tree>>(l<<2))&15);
			if (x <= 0) {
				mxIdx = l, x = 0;
			}
		}
		return mxIdx;
	}
	inline void parallel_build(int16_t A[], int16_t logN, int32_t inblock[]) {
		oA = A;
		int tmplogMs = MIN(logMs, logN), tmpMs = Ms;	
		int16_t *tbu = tb[0];
#pragma omp for schedule(static)
		for (int i = 0; i < M; i += POWS) {
			int16_t mx = 0;
			for (int j = 0; j < POWS; j++) {
				mx = MAX(mx, A[i+j]);
				pblock[i+j] = mx;
			}
			tbu[i>>LOGS] = mx;
		}

#pragma omp for schedule(static)
		for (int i = 0; i < M; i += POWS) {
			int16_t mx = 0;
			for (int j = POWS-1; j >= 0; j--) {
				mx = MAX(mx, A[i+j]);
				sblock[i+j] = mx;
			}
			if (GET(inblock, (i>>LOGS)))
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
			return oA[((l>>LOGS)<<LOGS)+RMQ(tree[l>>LOGS], l&(POWS-1), r&(POWS-1))];
		int16_t ret = 0;
		if ((r&(POWS-1)) != (POWS-1)) {
			ret = MAX(pblock[r], ret);
			r = r-1-(r&(POWS-1));
		}
		if (l&(POWS-1)) {
			ret = MAX(sblock[l], ret);
			l = l+POWS-(l&(POWS-1));
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
	static size_t f(int N, int logN) {
		const int M = ((N+POWS)>>LOGS)<<LOGS;
		const int Ms = M>>LOGS;
		const int logMs = MIN(log2int(Ms), logN);
		return sizeof(int16_t)*(logMs+1)*(Ms)
			+ sizeof(int16_t)*M*2
			+ sizeof(uint64_t)*Ms;
	}
};
//#undef LOGS
//#undef POWS

uint32_t seed = 0;
uint32_t p_func(uint32_t x) {return x*9301+49297;}
uint32_t p_random() {return seed = p_func(seed);}

int main() {
	int N, M, S, MOD;
	assert(scanf("%d %d %d %d", &N, &M, &S, &MOD) == 4);

	seed = S;

	int16_t A[MAXN] = {};
	int32_t L[MAXN] = {};
	for (int i = 1; i <= N; i++)
		A[i] = p_random()&MOD;
	for (int i = 1; i <= N; i++)	
		L[i] = i-(p_random()%min(i, MOD));

	int8_t logG[MAXN];
	int8_t logN = 0;
	int32_t inblock[(MAXN>>5)+1] = {};
	for (int i = 1; i <= N; i++) {
		int ql = L[i], qr = i;
		if (unlikely((ql>>LOGS) == (qr>>LOGS)))
			SET(inblock, (ql>>LOGS));
		if (ql&(POWS-1))
			ql = ql+(POWS-(ql&(POWS-1)));
		if ((qr&(POWS-1)) != (POWS-1))
			qr = qr-(qr&(POWS-1))-1;
		ql = ql>>LOGS;
		qr = qr>>LOGS;
		logG[i] = log2int(qr-ql+1);
		logN = MAX(logN, logG[i]);
	}
	logN = MIN(logN, log2int(N+1));
	miniSparseTable sp_tlb;
	{
	  int8_t *mem = (int8_t*) malloc(miniSparseTable::f(N, logN));
	  sp_tlb.init(N, logN, mem);
	  fprintf(stderr, "Compressed %zu\n", miniSparseTable::f(N, logN));
  }
	const int P = 20;
	omp_set_num_threads(P);
	int16_t hash  = 0;
#pragma omp parallel
	{
	  for (int it = 0; it < M; it++) {

		  sp_tlb.parallel_build(A, logN, inblock);

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
