#include <bits/stdc++.h>
using namespace std;

const int MAXN = 1024;

int run(int nA, char A[], short GA[], 
		int nB, char B[], short GB[]) {
	A--, B--, GA--, GB--;
	static short dp[MAXN][MAXN];
	memset(dp, 0, sizeof(dp));
	
	short ret = 0;
	for (int i = 1; i <= nA; i++) {
		for (int j = 1; j <= nB; j++) {
			if (A[i] == B[j]) {
				int ga = GA[i], gb = GB[j];
				short mx = 0;
				for (int p = min(ga+1, i); p >= 1; p--)
					for (int q = min(gb+1, j); q >= 1; q--)
					mx = max(mx, dp[i-p][j-q]);
				mx++;
				dp[i][j] = mx;
				ret = max(ret, mx);
			} else {
				dp[i][j] = 0;
			}
			printf("%d ", dp[i][j]);
		}
		puts("");
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
			scanf("%hd", &GA[i]);
		scanf("%s", B);
		nB = strlen(B);
		for (int i = 0; i < nB; i++)
			scanf("%hd", &GB[i]);
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

