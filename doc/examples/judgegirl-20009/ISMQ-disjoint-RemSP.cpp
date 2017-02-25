#include "ISMQ.h"
#include <algorithm>
#include <string.h>
using namespace std;

#define MAXN 16777216
static uint32_t value[MAXN], x;
static int32_t parent[MAXN], left[MAXN];

static int findp(int x) {
	return parent[x] == x ? x : (parent[x] = findp(parent[x]));
}
static inline int joint(int l, int r) {
	int x = l, y = r;
	while (parent[x] != parent[y]) {
		if (parent[x] < parent[y]) {
			if (x == parent[x]) {
				parent[x] = parent[y];
				break;
			}
			parent[x] = parent[y];
		} else {
			if (y == parent[y]) {
				parent[y] = parent[x];
				break;
			}
			parent[y] = parent[x];
		}
	}
	x = findp(x);
	left[x] = left[l];
	return left[x];
}

void init_ISMQ(int N) {
	for (int i = 0; i < N; i++)
		parent[i] = i;
	for (int i = 0; i < N; i++)
		left[i] = i;
	memset(value, 0, sizeof(value[0])*N);
	x = -1;
}
void append_ISMQ(uint32_t V) {
	++x;
	value[x] = V;
	int y = x-1;
	while (y >= 0) {
		y = findp(y);
		if (value[y] >= V)
			return ;
		y = joint(y, x);
		y--;
	}
}
uint32_t query_ISMQ(uint32_t L) {
	return value[findp(L)];
}

