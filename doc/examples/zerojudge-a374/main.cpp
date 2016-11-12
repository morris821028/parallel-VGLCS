#include <bits/stdc++.h>
using namespace std;

const int MAXN = 5005;

#define MIN(x, y) ((x) < (y) ? (x) : (y))
#define MAX(x, y) ((x) > (y) ? (x) : (y))
struct ISMQ {
	uint16_t *value;
	uint16_t *parent, *left;
	int findp(int x) {
        return parent[x] == x ? x : (parent[x] = findp(parent[x]));
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

int serial_VGLCS(int nA, char A[], uint16_t GA[],
        int nB, char B[], uint16_t GB[]) {
    A--, B--, GA--, GB--;
	assert(nA < MAXN);

	uint16_t *mem_base = (uint16_t *) malloc(sizeof(uint16_t)*(nA+1)*(nB+1)*3);
	uint16_t *mem_ismq = (uint16_t *) malloc(sizeof(uint16_t)*(nB+1)*3);
	assert(mem_base != NULL);
	assert(mem_ismq != NULL);
    ISMQ Q[MAXN];
    for (int i = 0; i <= nB; i++)
        Q[i].init(nA, mem_base+(i*(nA+1)*3));

    uint16_t ret = 0;
    for (int i = 1; i <= nA; i++) {
        int p_begin = i - MIN(GA[i]+1, i);
        ISMQ RQ;
        RQ.init(nB, mem_ismq);
        for (int j = 1; j <= nB; j++) {
			uint16_t tmp = 0;
            if (A[i] == B[j]) {
                tmp = RQ.get(j - MIN(GB[j]+1, j))+1;
            }
            RQ.set(j, Q[j].get(p_begin));
            Q[j].set(i, tmp);
            ret = MAX(ret, tmp);
        }
    }
	free(mem_base);
	free(mem_ismq);
    return ret;
}

int main() {
	static char A[MAXN], B[MAXN];
	static uint16_t GA[MAXN], GB[MAXN];
	int nA, nB, Q;
	while (scanf("%s %s", A, B) == 2) {
		nA = strlen(A);
		nB = strlen(B);
		scanf("%d", &Q);
		for (int i = 0; i < Q; i++) {
			int GLCS = 0, gap[128];
			char s[MAXN];
			for (int i = 0; i < 128; i++)
				gap[i] = 0x3f3f;
			while (scanf("%s", s) == 1 && s[0] != '$')
				scanf("%d", &gap[s[0]]);
			for (int i = 0; i < nA; i++)
				GA[i] = gap[A[i]];
			for (int i = 0; i < nB; i++)
				GB[i] = gap[B[i]];
			int ret = serial_VGLCS(nA, A, GA, nB, B, GB);
			printf("%d%c", ret, " \n"[i == Q-1]);
		}
	}
	return 0;
}

