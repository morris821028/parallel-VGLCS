#include "ISMQ.h"
#include <algorithm>
#include <string.h>
using namespace std;

#define MAXN 16777216
static uint32_t x;
struct Node {
	uint32_t value;
	int32_t parent, left;
} nodes[MAXN];
static int findp(int x) {
	return nodes[x].parent == x ? x : (nodes[x].parent = findp(nodes[x].parent));
}
static inline int joint(int l, int r) {
	l = findp(l);
	r = findp(r);
	nodes[l].parent = r, nodes[r].left = nodes[l].left;
	return nodes[r].left;
}

void init_ISMQ(int N) {
	for (int i = 0; i < N; i++) {
		nodes[i].parent = i;
		nodes[i].left = i;
		nodes[i].value = 0;
	}
	x = -1;
}
void append_ISMQ(uint32_t V) {
	++x;
	nodes[x].value = V;
	int y = x-1;
	while (y >= 0) {
		y = findp(y);
		if (nodes[y].value > V)
			return ;
		y = joint(y, x);
		y--;
	}
}
uint32_t query_ISMQ(uint32_t L) {
	return nodes[findp(L)].value;
}

