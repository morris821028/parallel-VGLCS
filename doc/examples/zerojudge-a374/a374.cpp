#include <bits/stdc++.h>
using namespace std;

const int MAXN = 801;

struct ISMQ {
	int16_t value[MAXN];
	int16_t parent[MAXN], left[MAXN];
	int findp(int x) {
        return parent[x] == x ? x : (parent[x] = findp(parent[x]));
    }
	inline int joint(int l, int r) {
		l = findp(l), r = findp(r);
		parent[l] = r, left[r] = left[l];
		return left[r];
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
int run(int nA, char A[], int16_t GA[], 
		int nB, char B[], int16_t GB[]) {
	A--, B--, GA--, GB--;
	if (nA > nB) {
		swap(GA, GB), swap(nA, nB), swap(A, B);
	}
	static ISMQ Q[MAXN];
	
	for (int i = 0; i <= nB; i++)
		Q[i].init(nA);
		
	int16_t ret = 0;
	for (int i = 1; i <= nA; i++) {
		int r = i - min(GA[i]+1, i);
		static ISMQ RQ;
		RQ.init(nB);
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
	static char A[MAXN], B[MAXN];
	static int16_t GA[MAXN], GB[MAXN];
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
			int ret = run(nA, A, GA, nB, B, GB);
			printf("%d%c", ret, " \n"[i == Q-1]);
		}
	}
	return 0;
}

