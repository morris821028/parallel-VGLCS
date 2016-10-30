#include <bits/stdc++.h>
using namespace std;

const int MAXN = 1024;

int run(int nA, char A[], int GA[], 
		int nB, char B[], int GB[]) {
	if (nA > nB) {
		swap(GA, GB), swap(nA, nB), swap(A, B);
	}
	A--, B--, GA--, GB--;
	static int dp[MAXN][MAXN];
	memset(dp, 0, sizeof(dp));
			
	int ret = 0;
	for (int i = 1; i <= nA; i++) {
		for (int j = 1; j <= nB; j++) {
			if (A[i] == B[j]) {
				int ga = GA[i], gb = GB[j], mx = 0;
				for (int p = min(ga+1, i); p >= 1; p--)
					for (int q = min(gb+1, j); q >= 1; q--)
						mx = max(mx, dp[i-p][j-q]);
				dp[i][j] = mx+1;
				ret = max(ret, mx+1);
			}
//			printf("%2d ", dp[i][j]);
		}
//		puts("");
	}
	return ret;
}

int main() {
	static char A[MAXN], B[MAXN];
	static int GA[MAXN], GB[MAXN];
	int nA, nB, Q;
	while (scanf("%s %s", A, B) == 2) {
		nA = strlen(A);
		nB = strlen(B);
		scanf("%d", &Q);
		for (int i = 0; i < Q; i++) {
			int GLCS = 0, gap[256];
			char s[MAXN];
			for (int i = 0; i < 256; i++)
				gap[i] = 0x3f3f3f;
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
/*
ACBDCAA ADDBCDBAC
2
$
A 2 B 0 C 3 D 0 $
*/

