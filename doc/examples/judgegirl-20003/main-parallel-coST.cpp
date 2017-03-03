#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <assert.h>
#include <algorithm>
#include <omp.h>
using namespace std;

static inline int log2int(int x) {
	return 31 - __builtin_clz(x);
}

#define MIN(x, y) ((x) < (y) ? (x) : (y))
#define MAX(x, y) ((x) > (y) ? (x) : (y))
#define MAXN 10005
#define MAXLOGN 15
#define _M64
// #define _M32
#if defined(_M64)
#define LOGS 4
#define POWS (1<<LOGS)
#define TYPE_TREE uint64_t
#elif defined(_M32)
#define LOGS 3
#define POWS (1<<LOGS)
#define TYPE_TREE uint32_t
#endif
#define GET(mark, x) (mark[(x)>>5]>>((x)&31)&1)
#define SET(mark, x) (mark[(x)>>5] |= 1<<((x)&31))

struct COST {
	int16_t *oA;
	int16_t *tb[MAXLOGN];
	int16_t *pblock, *sblock;
	TYPE_TREE *tree;
	int M, Ms, logMs, Mindex;
	/* --- dynamic --- */
	int16_t D[POWS+1], Dmx, B[POWS+1];
	int i, Dp;
	TYPE_TREE mask;

	static size_t size(int N, int logN, int hasRef) {
		int M = ((N+POWS)>>LOGS)<<LOGS;
		int Ms = M>>LOGS;
		int logMs = min(log2int(Ms), logN);
		size_t ret = 0;
		if (!hasRef)
			ret += sizeof(int16_t)*N;
		ret += (logMs+1) * sizeof(int16_t) * Ms;
		ret += sizeof(int16_t)*M;
		ret += sizeof(int16_t)*M;
		ret += sizeof(TYPE_TREE)*Ms;
		return ret;
	}
	void init(int N, int logN, int8_t *mem_base, int16_t *ref = NULL) {
		int8_t *mem = mem_base;
		M = ((N+POWS)>>LOGS)<<LOGS;
		Ms = M>>LOGS;
		logMs = min(log2int(Ms), logN);
		if (ref)
			oA = ref;
		else
			oA = (int16_t *) mem, mem += sizeof(int16_t)*N;
		for (int i = 0; i <= logMs; i++)
			tb[i] = (int16_t *) mem, mem += sizeof(int16_t)*(Ms);
		sblock = (int16_t *) mem, mem += sizeof(int16_t)*M;
		pblock = (int16_t *) mem, mem += sizeof(int16_t)*M;
		tree = (TYPE_TREE*) mem, mem += sizeof(TYPE_TREE)*Ms;
		/* --- dynamic --- */
		append_init();
	}
	void append_init() {
		D[0] = SHRT_MAX, Dmx = 0, i = 1, Dp = 0, mask = 0;
		Mindex = -1;
	}
	// parallel build
	static TYPE_TREE signature(int16_t A[]) { 
		int16_t D[POWS+1];
		D[0] = SHRT_MAX;
		TYPE_TREE mask = 0;
		for (int i = 1, Dp = 0; i <= POWS; i++, A++) {
			int16_t v = *A;
			int8_t cnt = 0;
			while (D[Dp] < v)
				Dp--, cnt++;
			D[++Dp] = v;
			mask = mask | (((TYPE_TREE) cnt)<<((i-1)<<2));
		}
		return mask;
	}
	static int8_t RMQ(TYPE_TREE tree, int8_t l, int8_t r) {
		int8_t mxIdx = l, x = 0;
		tree >>= (l)<<2;
		for (++l; l <= r; ++l) {
			tree >>= 1<<2;
			x = x+1 - (tree&15);
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
			{
				int16_t mx = 0;
				int16_t *tA = A+i;
				for (int j = 0; j < POWS; j++, tA++) {
					int16_t p = *tA;
					mx = MAX(p, mx);
					pblock[i+j] = mx;
				}
			}
			{
				int16_t mx = 0;
				int16_t *tA = A+i+POWS-1;
				for (int j = POWS-1; j >= 0; j--, tA--) {
					int16_t p = *tA;
					mx = MAX(mx, p);
					sblock[i+j] = mx;
				}
				tbu[i>>LOGS] = mx;
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

	void append(int16_t V, int32_t build_tree) {
		// begin coroutine
		++Mindex;
		Dmx = MAX(V, Dmx);
		pblock[Mindex] = Dmx, oA[Mindex] = V, B[i] = V;
		if (build_tree) {
			int cnt = 0;
			while (D[Dp] < V)
				Dp--, cnt++;
			D[++Dp] = V;
			mask = mask | (((TYPE_TREE) cnt)<<((i-1)<<2));
			tree[Mindex>>LOGS] = mask;
		}
		i++;
		// end

		if ((Mindex&(POWS-1)) == (POWS-1)) {

			int16_t Bmx = 0;

			for (int it = POWS, j = 0; it >= 1; it--, j++) {
				int16_t t = B[it];
				Bmx = MAX(Bmx, t);
				sblock[Mindex-j] = Bmx;
			}

			int pos = Mindex>>LOGS;
			tb[0][pos] = Bmx;
			for (int i = 1, before = pos-1; before >= 0 && i <= logMs; i++) {
				int16_t p = tb[i-1][before];
				int16_t q = tb[i-1][pos];
				tb[i][pos] = MAX(q, p);
				before -= 1<<(i-1);
			}
			mask = 0, i = 1, Dp = 0, Dmx = 0;
		}
	}
	inline int16_t get(int l, int r, int t) {
		if ((l>>LOGS) == (r>>LOGS))
			return oA[((l>>LOGS)<<LOGS)+RMQ(tree[l>>LOGS], l&(POWS-1), r&(POWS-1))];
		int16_t ret = 0;
		if ((r&(POWS-1)) != (POWS-1)) {
			ret = pblock[r];
			r = r-1-(r&(POWS-1));
		}
		int tmp = -1;
		if (l&(POWS-1)) {
			tmp = l;
			// ret = MAX(sblock[l], ret);
			l = l+POWS-(l&(POWS-1));
			if (l > r) {
				int16_t t = sblock[tmp];
				ret = MAX(ret, t);
			}
		}
		if (l > r)
			return ret;
		l = l>>LOGS;
		r = r>>LOGS;
		int16_t *tbv = tb[t];
		int16_t p = tbv[l+(1<<t)-1];
		int16_t q = tbv[r];
		ret = MAX(p, ret);
		ret = MAX(q, ret);
		if (tmp >= 0) {
			if (tb[0][tmp>>LOGS] <= ret)
				return ret;
			int16_t t = sblock[tmp];
			ret = MAX(ret, t);
		}
		return ret;
	}
};
int parallel_VGLCS(int nA, char A[], int16_t GA[],
		int nB, char B[], int16_t GB[]) {
	A--, B--, GA--, GB--;
	if (nA > nB) {
		std::swap(GA, GB), std::swap(nA, nB), std::swap(A, B);
	}
	assert(nA < MAXN && nB < MAXN);

	COST Q[MAXN];
	COST sp_tlb;
	static int16_t spA[MAXN];
	char logGB[MAXN], logGA[MAXN];
	int16_t ret = 0;

	int16_t logQA = 0, logQB = 0;
	int32_t inblockA[(MAXN>>5)+5] = {};
	int32_t inblockB[(MAXN>>5)+5] = {};

	for (int i = 1; i <= nA; i++) {
		int ql = i - MIN(GA[i]+1, i), qr = i-1;
		if ((ql>>LOGS) == (qr>>LOGS))
			SET(inblockA, (ql>>LOGS));
		if (ql&(POWS-1))
			ql = ql+(POWS-(ql&(POWS-1)));
		if ((qr&(POWS-1)) != (POWS-1))
			qr = qr-(qr&(POWS-1))-1;
		ql = ql>>LOGS;
		qr = qr>>LOGS;
		logGA[i] = log2int(qr-ql+1);
		logQA = MAX(logQA, logGA[i]);
	}
	for (int i = 1; i <= nB; i++) {
		int ql = i - MIN(GB[i]+1, i), qr = i-1;
		if ((ql>>LOGS) == (qr>>LOGS))
			SET(inblockB, (ql>>LOGS));
		if (ql&(POWS-1))
			ql = ql+(POWS-(ql&(POWS-1)));
		if ((qr&(POWS-1)) != (POWS-1))
			qr = qr-(qr&(POWS-1))-1;
		ql = ql>>LOGS;
		qr = qr>>LOGS;
		logGB[i] = log2int(qr-ql+1);
		logQB = MAX(logQB, logGB[i]);
	}

	const int P = 20;
	const int lognA = MIN(log2int(nA+1), logQA);
	const int lognB = MIN(log2int(nB+1), logQB);
	// const int lognA = log2int(nA+1);
	// const int lognB = log2int(nB+1);
	const size_t usageMemA = COST::size(nA+1, lognA, false); 
	const size_t usageMemB = COST::size(nB+1, lognB, true); 
	static int8_t *mem_tlbD = NULL, lastUsageMemA = 0;
	if (lastUsageMemA < usageMemA*(nB+2)) {
		if (mem_tlbD != NULL)	free(mem_tlbD);
		mem_tlbD = (int8_t *) malloc(usageMemA*(nB+2));
		lastUsageMemA = usageMemA*(nB+2);
	}
	static int8_t *mem_tlbR = NULL, lastUsageMemB = 0;
	if (lastUsageMemB < usageMemB) {
		if (mem_tlbR != NULL)	free(mem_tlbR);
		mem_tlbR = (int8_t *) malloc(usageMemB);
		lastUsageMemB = usageMemB;
	}
	{
		assert(mem_tlbD != NULL);
		assert(mem_tlbR != NULL);
		sp_tlb.init(nB, lognB, mem_tlbR, spA);
		omp_set_num_threads(P);
	}

#pragma omp parallel
	{   
		// init ISMQ
#pragma omp for schedule(static)
		for (int i = 0; i <= nB; i++) {
			Q[i].init(nA+1, lognA, mem_tlbD + (i*usageMemA), NULL);
			Q[i].append(0, GET(inblockA, (0>>LOGS)));
		}

		for (int i = 1; i <= nA; i++) {
			// query: incremental suffix maximum query
			{
				int r = i - MIN(GA[i]+1, i);
				int16_t *tb = sp_tlb.tb[0];
				char logG = logGA[i];
#pragma omp for schedule(static)
				for (int j = 1; j <= nB; j++)
					spA[j] = Q[j].get(r, i-1, logG);
			}

			// doubling algorithm
			sp_tlb.parallel_build(spA, lognB, inblockB);

			char Ai = A[i];
			int32_t build_tree = GET(inblockA, (i>>LOGS));
			// dynamic programming
#pragma omp for schedule(static) reduction(max: ret)
			for (int j = 1; j <= nB; j++) {
				if (Ai == B[j]) {
					int l = j - MIN(GB[j]+1, j), r = j-1;
					int16_t val = sp_tlb.get(l, r, logGB[j])+1;
					Q[j].append(val, build_tree);
					ret = MAX(ret, val);
				} else {
					Q[j].append(0, build_tree);
				}
			}

		}
	}

//	free(mem_tlbD);
//	free(mem_tlbR);
	return ret;
}

int main() {
	char A[MAXN], B[MAXN];
	int16_t GA[MAXN], GB[MAXN];
	int nA, nB, Q;
	while (scanf("%s", A) == 1) {
		nA = strlen(A);
		for (int i = 0; i < nA; i++)
			assert(scanf("%hd", &GA[i]) == 1);
		assert(scanf("%s", B) == 1);
		nB = strlen(B);
		for (int i = 0; i < nB; i++)
			assert(scanf("%hd", &GB[i]) == 1);
		int ret = parallel_VGLCS(nA, A, GA, nB, B, GB);
		printf("%d\n", ret);
	}
	return 0;
}
