#include "ISMQ.h"
#include <bits/stdc++.h>
#include <algorithm>
using namespace std;

#define MAXN 16777216
static int8_t mem[512<<20];
#define MAXLOGN 24
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
#define likely(x) __builtin_expect(!!(x), 1)
#define unlikely(x) __builtin_expect(!!(x), 0)

static uint32_t oA[MAXN];
static uint32_t *tb[MAXLOGN];
static uint32_t *pblock, *sblock;
static TYPE_TREE *tree;
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
	sblock = (uint32_t *) mem, mem += sizeof(uint32_t)*M;
	pblock = (uint32_t *) mem, mem += sizeof(uint32_t)*M;
	tree = (TYPE_TREE*) mem, mem += sizeof(TYPE_TREE)*Ms;
}
static inline int8_t RMQ(TYPE_TREE tree, int8_t l, int8_t r) {
	int8_t mxIdx = l, x = 0;
	for (++l; l <= r; ++l) {
		x = x+1 - ((tree>>(l<<2))&15);
		if (x <= 0) {
			mxIdx = l, x = 0;
		}
	}
	return mxIdx;
}
void init_ISMQ(int N) {
	Mindex = -1;
	int logN = log2int(N);
	init(N, logN, mem);
}

void append_ISMQ(uint32_t V) {
	static uint32_t D[POWS+1] = {UINT_MAX}, Dmx = 0, B[POWS+1];
	static int i = 1, Dp = 0;
	static TYPE_TREE mask = 0;
	// begin coroutine
	++Mindex;
	Dmx = max(Dmx, V), oA[Mindex] = V, pblock[Mindex] = Dmx;
	B[i] = V;
	int cnt = 0;
	while (D[Dp] < V)
		Dp--, cnt++;
	D[++Dp] = V;
	mask = mask | (((TYPE_TREE) cnt)<<((i-1)<<2));
	i++;
	// end
	tree[Mindex>>LOGS] = mask;	

	if ((Mindex&(POWS-1)) == (POWS-1)) {

		uint32_t Bmx = 0;

        for (int it = i-1, j = 0; it >= 1; it--, j++) {
            Bmx = max(Bmx, B[it]);
            sblock[Mindex-j] = Bmx;
        }

		int pos = Mindex>>LOGS;
		tb[0][pos] = Dmx;
		for (int i = 1, before = pos-1; before >= 0 && i <= logMs; i++) {
			uint32_t p = tb[i-1][before];
			uint32_t q = tb[i-1][pos];
			tb[i][pos] = max(q, p);
			before -= 1<<(i-1);
		}
		mask = 0, i = 1, Dp = 0, Dmx = 0;
	}
}
uint32_t query_ISMQ(uint32_t l) {
	uint32_t r = Mindex;
	if ((l>>LOGS) == (r>>LOGS))
		return oA[((l>>LOGS)<<LOGS)+RMQ(tree[l>>LOGS], l&(POWS-1), r&(POWS-1))];
	uint32_t ret = 0;
	if ((r&(POWS-1)) != (POWS-1)) {
		ret = pblock[r];
		r = r-1-(r&(POWS-1));
	}
	int tmp = -1;
	if (l&(POWS-1)) {
		tmp = l;
		l = l+POWS-(l&(POWS-1));
		if (l > r)
		ret = max(ret, sblock[tmp]);
	}
	if (unlikely(l > r))
		return ret;

	l = l>>LOGS, r = r>>LOGS;
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

