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
			int tmp = parent[x];
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


static inline int log2int(int x) {
    return __builtin_clz((int)1) - __builtin_clz(x);
}
int parallel_VGLCS(int nA, char A[], short GA[], 
					int nB, char B[], short GB[]) {
	A--, B--, GA--, GB--;
	if (nA > nB) {
		std::swap(GA, GB), std::swap(nA, nB), std::swap(A, B);
	}
	static ISMQ Q[MAXN];

	short ret = 0;
	const int lognB = log2int(nB);
	
	short tb[MAXLOGN][MAXN] = {};
	omp_set_num_threads(20);
	const int chunk = MAX(nB / 20, 1);

	#pragma omp parallel
	{
		// init ISMQ
		#pragma omp for schedule(static, chunk)
		for (int i = 0; i <= nB; i++)
			Q[i].init(nA);

		char tmpB[MAXN];
		for (int i = 1; i <= nB; i++)
			tmpB[i] = B[i];
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
						tbu[j] = MAX(tbv[j-(1<<(k-1))], tbv[j]);
					}
				}
			}

			// dynamic programming
			#pragma omp for schedule(static, chunk) reduction(max: ret)
			for (int j = 1; j <= nB; j++) {
				if (Ai == tmpB[j]) {
					int l = j - MIN(GB[j]+1, j), r = j-1;
					int t = log2int(r-l+1);
					short val = MAX(tb[t][l+(1<<t)-1], tb[t][r])+1;
					Q[j].set(i, val);
					ret = MAX(ret, val);
				}
			}
			
/*
			// update: incremental suffix maximum query
			#pragma omp for schedule(static, chunk) reduction(max: ret)
			for (int j = 1; j <= nB; j++) {
				Q[j].set(i, dp[j]);
				ret = MAX(ret, dp[j]);
			}
*/
		}
	}
	return ret;
}
#undef MIN
#undef MAX
