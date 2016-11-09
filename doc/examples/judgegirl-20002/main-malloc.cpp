#include <bits/stdc++.h>
#include <time.h>
using namespace std;

struct ISMQ {
	int16_t *value;
	int16_t *parent, *left;
	int findp(int x) {
        return parent[x] == x ? x : (parent[x] = findp(parent[x]));
    }
	inline int joint(int l, int r) {
		l = findp(l);
		r = findp(r);
		parent[l] = r, left[r] = left[l];
		return left[r];
	}
	void init(int n, int16_t *mem_base) {
		parent = mem_base;
		left = mem_base+n+1;
		value = mem_base+(n+1)*2;
		int16_t *tmp;
		tmp = parent;
		for (int i = 0; i <= n; i++)
			*tmp = i, tmp++;
		tmp = left;
		for (int i = 0; i <= n; i++)
			*tmp = i, tmp++;
		memset(value, 0, sizeof(int16_t)*(n+1));
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

int16_t *mem_base, *mem_ismq;
int serial_VGLCS(int nA, char A[], int16_t GA[],
        int nB, char B[], int16_t GB[]) {
    A--, B--, GA--, GB--;
	const int MAXN = 5005;
	assert(nA < MAXN);

	assert(mem_base != NULL);
	assert(mem_ismq != NULL);

    static ISMQ Q[MAXN];
    for (int i = 0; i <= nB; i++)
        Q[i].init(nA, mem_base+(i*(nA+1)*3));

    int16_t ret = 0;
    for (int i = 1; i <= nA; i++) {
        int r = i - min(GA[i]+1, i);
        static ISMQ RQ;
        RQ.init(nB, mem_ismq);
        for (int j = 1; j <= nB; j++) {
			int16_t tmp = 0;
            if (A[i] == B[j]) {
                tmp = RQ.get(j - min(GB[j]+1, j))+1;
            	ret = max(ret, tmp);
            }
			int16_t Qr = Q[j].get(r);
			if (Qr)
            	RQ.set(j, Qr);
			if (tmp)
	            Q[j].set(i, tmp);
        }
    }
    return ret;
}

int main() {
	static const int MAXN = 5005;
	static char A[MAXN], B[MAXN];
	static int16_t GA[MAXN], GB[MAXN];
	int nA, nB, Q;
	while (scanf("%s %s", A, B) == 2) {
		nA = strlen(A);
		nB = strlen(B);
		scanf("%d", &Q);
		mem_base = (int16_t *) malloc(sizeof(int16_t)*(nA+1)*(nB+1)*3);
		mem_ismq = (int16_t *) malloc(sizeof(int16_t)*(nB+1)*3);
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
		free(mem_base);
		free(mem_ismq);
	}
	return 0;
}

