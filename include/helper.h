#ifndef HELPER_H
#define HELPER_H
#include "config.h"
#define MIN(x, y) ((x) < (y) ? (x) : (y))
#define MAX(x, y) ((x) > (y) ? (x) : (y))

static inline int log2int(int x) {
	return __builtin_clz((int) 1) - __builtin_clz(x); 
}

struct ISMQ {
	uint16_t *value;
	uint16_t *parent, *left;
	int findp(int x) {
		int u = x;
		while (u != parent[u])
			u = parent[u];
		while (x != parent[x]) {
			uint16_t tmp = parent[x];
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
	void init(int n, uint16_t *mem_base) {
		parent = mem_base;
		left = mem_base+n+1;
		value = mem_base+(n+1)*2;
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

struct SparseTable {
	uint16_t *tb[MAXLOGN];
	// create tb[0..logN][0..n]
	inline void init(int n, int logN, uint16_t *mem_base) {
		for (int i = 0; i <= logN; i++) {
			tb[i] = mem_base + i*(n+1);
			tb[i][0] = 0;
		}
	}
	// parallel build 
	inline void parallel_build(int n, int logN) {
		for (int k = 1; k <= logN; k++) {
			uint16_t *tbu = tb[k];
			const uint16_t *tbv = tb[k-1];
			#pragma omp for schedule(static)
			for (int i = 1; i <= n; i++) {
				if (i-(1<<(k-1)) >= 0) {
					uint16_t p = tbv[i-(1<<(k-1))];
					uint16_t q = tbv[i];
					tbu[i] = MAX(q, p);
				}
			}
		}
	}
	// set tb[0][x] = val, and update its relationship
	inline void set(int x, uint16_t val, int limG) {
		tb[0][x] = val;
		for (int i = 1; i <= limG && (1<<(i-1)) <= x; i++) {
			uint16_t p = tb[i-1][x-(1<<(i-1))];
			uint16_t q = tb[i-1][x];
			tb[i][x] = MAX(q, p);
		}
	}
	// query the maximum value of interval [l..r]
	inline uint16_t get(int l, int r) {
		int t = log2int(r-l+1);
		uint16_t p = tb[t][l+(1<<t)-1];
		uint16_t q = tb[t][r];
		return MAX(q, p);
	}
	inline uint16_t get(int l, int r, int t) {
		uint16_t p = tb[t][l+(1<<t)-1];
		uint16_t q = tb[t][r];
		return MAX(q, p);
	}
};

#undef MIN
#undef MAX
#endif
