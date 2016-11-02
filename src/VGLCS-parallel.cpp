#include <stdlib.h>
#include <string.h>
#include <algorithm>
#include <omp.h>
#include "VGLCS.h"

const int MAXN = 5005;
const int MAXLOGN = 16;
struct ISMQ {
	short value[MAXN];
	short parent[MAXN], left[MAXN];
	int findp(int x) {
		int u = x;
		while (u != parent[u])
			u = parent[u];
		while (x != parent[x]) {
			short tmp = parent[x];
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
	void init(int n) {
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
int serial_VGLCS(int nA, char A[], short GA[],
        int nB, char B[], short GB[]) {
    A--, B--, GA--, GB--;
    static ISMQ Q[MAXN];

    for (int i = 0; i <= nB; i++)
        Q[i].init(nA);

    short ret = 0;
    for (int i = 1; i <= nA; i++) {
        int p_begin = i - MIN(GA[i]+1, i);
        short dp[MAXN] = {};
        ISMQ RQ;
        RQ.init(nB);
        for (int j = 1; j <= nB; j++) {
            if (A[i] == B[j]) {
                dp[j] = RQ.get(j - MIN(GB[j]+1, j))+1;
            } else {
                dp[j] = 0;
            }
            RQ.set(j, Q[j].get(p_begin));
        }
        for (int j = 1; j <= nB; j++) {
            ret = MAX(ret, dp[j]);
            Q[j].set(i, dp[j]);
        }
    }
    return ret;
}

int parallel_VGLCS(int nA, char A[], short GA[], 
					int nB, char B[], short GB[]) {
	A--, B--, GA--, GB--;
	if (nA > nB) {
		std::swap(GA, GB), std::swap(nA, nB), std::swap(A, B);
	}
	static ISMQ Q[MAXN];

	short ret = 0, max_gap = 0;
	for (int i = 1; i <= nB; i++)
		max_gap = MAX(max_gap, GB[i]);
	const int lognB = log2int(max_gap+1);

	short tb[MAXLOGN][MAXN] = {};
	const int P = 20;
	omp_set_num_threads(P);
	const int chunk = MAX((nB+P-1) / P, 1);
	
	char logGB[MAXN];
	for (int i = 1; i <= nB; i++) {
		int l = i - MIN(GB[i]+1, i), r = i-1;
		logGB[i] = log2int(r-l+1);
	}

	#pragma omp parallel
	{
		// init ISMQ
		#pragma omp for schedule(static, chunk)
		for (int i = 0; i <= nB; i++)
			Q[i].init(nA);
		// copy to stack
		char tmpB[MAXN];
		memcpy(tmpB, B, sizeof(char)*(nB+1));
		
		for (int i = 1; i <= nA; i++) {
			int p_begin = i - MIN(GA[i]+1, i);
			char Ai = A[i];
			// query: incremental suffix maximum query
			#pragma omp for schedule(static, chunk)
			for (int j = 1; j <= nB; j++)
				tb[0][j] = Q[j].get(p_begin);
				
			// doubling algorithm
			for (int k = 1; k <= lognB; k++) {
				short *tbu = tb[k];
				const short *tbv = tb[k-1];
				#pragma omp for schedule(static, chunk)
				for (int j = 1; j <= nB; j++) {
					if (j-(1<<(k-1)) >= 0) {
						const short p = tbv[j-(1<<(k-1))];
						const short q = tbv[j];
						tbu[j] = MAX(q, p);
					}
				}
			}

			// dynamic programming
			#pragma omp for schedule(static, chunk) reduction(max: ret)
			for (int j = 1; j <= nB; j++) {
				if (Ai == tmpB[j]) {
					int l = j - MIN(GB[j]+1, j), r = j-1;
					int t = logGB[j];
					const short p = tb[t][l+(1<<t)-1];
					const short q = tb[t][r];
					short val = MAX(q, p)+1;
					Q[j].set(i, val);
					ret = MAX(ret, val);
				}
			}
		}
	}
	return ret;
}
#undef MIN
#undef MAX
