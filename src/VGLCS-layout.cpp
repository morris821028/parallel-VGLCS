#include <bits/stdc++.h>
#include <omp.h>
using namespace std;

const int MAXN = 5005;
const int MAXLOGN = 16;
class ISMQ {
private:
	short value[MAXN];
	short parent[MAXN], left[MAXN];
/*
	int findp(int x) {
		return parent[x] == x ? x : (parent[x] = findp(parent[x]));
	}
*/
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
		l = findp(l), r = findp(r);
		parent[l] = r, left[r] = left[l];
		return left[r];
	}
public:
	void init(int n) {
		for (int i = 0; i <= n; i++)
			parent[i] = i;
		for (int i = 0; i <= n; i++)
			left[i] = i;
//		for (int i = 0; i <= n; i++)
//			value[i] = 0;
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
static inline int log2int(int x){
    return __builtin_clz((int)1)-__builtin_clz(x);
}
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
	const int lognB = log2int(nB);
	
	short dp[MAXN] = {};
	short tb[MAXN][MAXLOGN] = {};
	omp_set_num_threads(20);
	const int chunk = nB / 20;
	#pragma omp parallel
	{
		char tmpB[MAXN];
		for (int i = 1; i <= nB; i++)
			tmpB[i] = B[i];
		for (int i = 1; i <= nA; i++) {
			int p_begin = i - min(GA[i]+1, i);
			char Ai = A[i];
			// query: incremental suffix maximum query
			#pragma omp for schedule(static, chunk)
			for (int j = 1; j <= nB; j++)
				tb[j][0] = Q[j].get(p_begin);
				
			// doubling algorithm
			for (int k = 1; k <= lognB; k++) {
				#pragma omp for schedule(static, chunk)
				for (int j = 1<<(k-1); j <= nB; j++) {
					if (j-(1<<(k-1)) >= 0) {
						tb[j][k] = max(tb[j][k-1], tb[j-(1<<(k-1))][k-1]);
					}
				}
			}

			// dynamic programming
			#pragma omp for schedule(static, chunk)
			for (int j = 1; j <= nB; j++) {
				if (Ai == tmpB[j]) {
					int l = j - min(GB[j]+1, j), r = j-1;
					int t = log2int(r-l+1);
					dp[j] = max(tb[l+(1<<t)-1][t], tb[r][t])+1;
				} else {
					dp[j] = 0;
				}
			}
			
			// update: incremental suffix maximum query
			#pragma omp for schedule(static, chunk) reduction(max: ret)
			for (int j = 1; j <= nB; j++) {
				Q[j].set(i, dp[j]);
				ret = max(ret, dp[j]);
			}
		}
	}
	return ret;
}

int main() {
//	freopen("2.in", "r", stdin);
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

