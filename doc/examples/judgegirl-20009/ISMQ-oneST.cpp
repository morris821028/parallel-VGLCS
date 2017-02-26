#include "ISMQ.h"
#include <bits/stdc++.h>
#include <algorithm>
using namespace std;

#define MAXN 16777216
static int8_t mem[512<<20];
#define MAXLOGN 24
#define LOGS 3
#define POWS (1<<LOGS)
#define likely(x) __builtin_expect(!!(x), 1)
#define unlikely(x) __builtin_expect(!!(x), 0)

// Cartesian Tree, $C_8 = 1430, C_0 + C_1 + \cdot + C_8 = 2056$
static int Cn[POWS+1] = {1, 1, 2, 5, 14, 42, 132, 429, 1430};
static int CnBase[POWS+1][POWS+1];
struct T {
	int8_t v[POWS][POWS];
} _mem[2100];
static T *LCA[MAXN];
static bool hasBuildLCA = false;
static void buildLCA() {
	hasBuildLCA = true;
	// catalan number and prefix sum of catalan number
	CnBase[1][1] = 1;
	for (int i = 1; i <= POWS; i++) {
		int ret = 0;
		for (int j = 0; j < i; j++)
			CnBase[i][i-j-1] = ret, ret += Cn[j]*Cn[i-1-j];
		CnBase[i][i] = ret;
	}
	// create LCA for each binary search tree
	LCA[0] = _mem;
	LCA[1] = _mem+1;
	for (int i = 2, off = 1; i <= POWS; i++) {
		off += Cn[i-1];
		LCA[i] = _mem + off;
		{
			// prepare
			const int l = 0, r = Cn[i];
			int lsz = i-1, rsz = 0, lid = 0, rid = 0;
			// start
			T *store = LCA[i];
			for (int j = l; j < r; j++) {
				for (int p = 0; p < lsz; p++)
					for (int q = p; q < lsz; q++)
						store[j].v[p][q] = LCA[lsz][lid].v[p][q];
				for (int p = lsz+1; p < i; p++)
					for (int q = p; q < i; q++)
						store[j].v[p][q] = LCA[rsz][rid].v[p-lsz-1][q-lsz-1]+lsz+1;
				for (int p = 0; p <= lsz; p++)
					for (int q = lsz; q < i; q++)
						store[j].v[p][q] = lsz;
				rid++;
				if (rid == Cn[rsz]) {
					rid = 0, lid++;
					if (lid == Cn[lsz])
						lid = 0, lsz--, rsz++;
				}
			}
		}
	}
}
static inline int tid(int lsz, int lid, int rsz, int rid) {
	if (rsz == 0)	return lid;
	return CnBase[lsz+rsz+1][lsz] + lid*Cn[rsz] + rid;
}
struct State {
	int i, s, tid, Dp;
	uint32_t D[POWS+1][4];
	State() {init();}
	inline void init() {
		i = 1, s = POWS, tid = Cn[POWS]-1, Dp = 0;
		D[0][0] = UINT_MAX;
	}
};
static void typeOfCartesian(State &state, uint32_t v) {
	int Dp = state.Dp;
	int lsz = 0;
	int lid = 0;
	int bsz = state.s-state.i+1;
	int bid = Cn[bsz]-1;
	while (state.D[Dp][0] < v) {
		lid = tid(state.D[Dp][1], state.D[Dp][2], lsz, lid);
		lsz += state.D[Dp][1]+1;
		bid = tid(state.D[Dp][1], state.D[Dp][2], bsz, bid);
		bsz += state.D[Dp][1]+1;
		Dp--;
	}
	Dp++;
	int ins = bid;
	int _tid = tid(lsz, lid, state.s-state.i, Cn[state.s-state.i]-1);
	ins = _tid - ins;
	state.D[Dp][0] = v, state.D[Dp][1] = lsz, state.D[Dp][2] = lid, state.D[Dp][3] = _tid;
	state.i++;
	state.Dp = Dp;
	state.tid += ins;
}

// Sparse Table
static uint32_t oA[MAXN];
static uint32_t *tb[MAXLOGN];
static uint32_t *pblock, *sblock;
static int16_t *tree;
static int M, Ms, logMs;
static int Mindex;
static inline int log2int(int x) {
	return 31 - __builtin_clz(x);
}
inline void init(int N, int logN, int8_t *mem) {
	M = ((N+POWS)>>LOGS)<<LOGS;
	Ms = M>>LOGS;
	logMs = min(log2int(Ms), logN);
	for (int i = 0; i <= logMs; i++)
		tb[i] = (uint32_t *) mem, mem += sizeof(uint32_t)*(Ms);
	pblock = (uint32_t *) mem, mem += sizeof(uint32_t)*M;
	sblock = (uint32_t *) mem, mem += sizeof(uint32_t)*M;
	tree = (int16_t*) mem, mem += sizeof(int16_t)*Ms;
}
static inline int8_t RMQ(int16_t tree, int8_t l, int8_t r) {
	return LCA[POWS][tree].v[l][r];
}
void init_ISMQ(int N) {
	Mindex = -1;
	int logN = log2int(N);
	init(N, logN, mem);
	if (!hasBuildLCA)
		buildLCA();
}

void append_ISMQ(uint32_t V) {
	static State t;
	static uint32_t Dmx = 0, B[POWS+1];
	// begin coroutine
	++Mindex;
	Dmx = max(Dmx, V), B[t.i] = V, oA[Mindex] = V, pblock[Mindex] = Dmx;
	typeOfCartesian(t, V);
	// end
	tree[Mindex>>LOGS] = t.tid;

	if ((Mindex&(POWS-1)) == (POWS-1)) {
		uint32_t Bmx = 0;
		for (int it = t.i-1, j = 0; it >= 1; it--, j++) {
			Bmx = max(Bmx, B[it]);
			sblock[Mindex-j] = Bmx;
		}
		int pos = Mindex>>LOGS;
		tb[0][pos] = Bmx;
		for (int i = 1, before = pos-1; before >= 0 && i <= logMs; i++) {
			uint32_t p = tb[i-1][before];
			uint32_t q = tb[i-1][pos];
			tb[i][pos] = max(p, q);
			before -= 1<<(i-1);
		}
		t.init(), Dmx = 0;
	}
}
uint32_t query_ISMQ(uint32_t l) {
	uint32_t r = Mindex;
	if (unlikely((l>>LOGS) == (r>>LOGS)))
		return oA[((l>>LOGS)<<LOGS)+RMQ(tree[l>>LOGS], l&(POWS-1), r&(POWS-1))];
	uint32_t ret = 0;
	if ((r&(POWS-1)) != (POWS-1)) {
		ret = max(pblock[r], ret);
		r = r-1-(r&(POWS-1));
	}
	int tmp = -1;
	if (l&(POWS-1)) {
		// ret = max(sblock[l], ret);
		tmp = l;
		l = l+POWS-(l&(POWS-1));
		if (l > r)
		ret = max(ret, sblock[tmp]);
	}
	if (unlikely(l > r))
		return ret;

	l = l>>LOGS;
	r = r>>LOGS;
	int t = log2int(r-l+1);
	uint32_t p = tb[t][l+(1<<t)-1];
	uint32_t q = tb[t][r];
	ret = max(p, ret);
	ret = max(q, ret);
	if (tmp >= 0 && tb[0][tmp>>LOGS] <= ret)
		return ret;
	if (tmp >= 0)
		ret = max(ret, sblock[tmp]);
	return ret;
}

