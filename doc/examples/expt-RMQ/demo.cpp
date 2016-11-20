#include <bits/stdc++.h>
using namespace std;
//#define DEBUG
const int MAXN = 100005;
const int MAXLOGN = 20;
uint32_t seed = 0;
uint32_t p_func(uint32_t x) {return x*9301+49297;}
uint32_t p_random() {return seed = p_func(seed);}

int main() {
	int N, M, S, MOD;
	assert(scanf("%d %d %d %d", &N, &M, &S, &MOD) == 4);

	seed = S;

	int16_t A[MAXN] = {}, L[MAXN] = {};
	for (int i = 1; i <= N; i++)
		A[i] = p_random()&MOD;
	for (int i = 1; i <= N; i++)	
		L[i] = i-(p_random()%min(i, MOD));

	uint16_t hash = 0;
	for (int i = 0; i < M; i++) {
#ifdef DEBUG
		for (int j = 1; j <= N; j++)
			printf("%d ", A[j]);
		puts("");
#endif
		for (int j = 1; j <= N; j++) {
			int l = L[j], r = j;
			int16_t mx = 0;
			for (int k = l; k <= r; k++)
				mx = max(mx, A[k]);
			hash ^= mx;
#ifdef DEBUG
			printf("query max(A[%d %d]) = %d\n", l, r, mx);
#endif
		}
		for (int j = 1; j <= N; j++)
			A[j] = p_func(A[j])&MOD;
	}
	printf("%X\n", hash);

	return 0;
}

