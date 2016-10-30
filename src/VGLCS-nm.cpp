#include <bits/stdc++.h>
using namespace std;

const int MAXN = 5005;

class ISMQ {
private:
	short value[MAXN];
	short parent[MAXN], left[MAXN];
	int findp(int x) {
		return parent[x] == x ? x : (parent[x] = findp(parent[x]));
	}
	inline int joint(int l, int r) {
		l = findp(l), r = findp(r);
		parent[l] = r, left[r] = left[l];
		return left[l];
	}
public:
	void init(int n) {
		for (int i = 0; i <= n; i++)
			parent[i] = i, left[i] = i, value[i] = 0;
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
int run(int nA, char A[], short GA[], 
		int nB, char B[], short GB[]) {
	A--, B--, GA--, GB--;
	if (nA > nB) {
		swap(GA, GB), swap(nA, nB), swap(A, B);
	}
	static ISMQ Q[MAXN];
	
	for (int i = 0; i <= nB; i++)
		Q[i].init(nA);
		
	short ret = 0;
	for (int i = 1; i <= nA; i++) {
		int p_begin = i - min(GA[i]+1, i);
		short dp[MAXN] = {};
		ISMQ RQ;
		RQ.init(nB);
		for (int j = 1; j <= nB; j++) {
			if (A[i] == B[j]) {
				dp[j] = RQ.get(j - min(GB[j]+1, j))+1;
			} else {
				dp[j] = 0;
			}
//			printf("%d ", dp[j]);
			RQ.set(j, Q[j].get(p_begin));
		}
//		puts("");
		for (int j = 1; j <= nB; j++) {
			ret = max(ret, dp[j]);
			Q[j].set(i, dp[j]);
		}
	}
	return ret;
}

int main() {
	static char A[MAXN], B[MAXN];
	static short GA[MAXN], GB[MAXN];
	int nA, nB;
	while (scanf("%s", A) == 1) {
		nA = strlen(A);
		for (int i = 0; i < nA; i++)
			assert(scanf("%hd", &GA[i]) == 1);
		assert(scanf("%s", B) == 1);
		nB = strlen(B);
		for (int i = 0; i < nB; i++)
			assert(scanf("%hd", &GB[i]) == 1);
		int ret = run(nA, A, GA, nB, B, GB);
		printf("%d\n", ret);
	}
	return 0;
}
/*
RCLPCRR
2 3 0 0 3 2 2
RPPLCPLRC
2 0 0 0 3 0 0 2 3
*/

