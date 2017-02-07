#include <bits/stdc++.h>
#include "ISMQ.h"

using namespace std;

static uint32_t seed = 0;
static uint32_t p_func(uint32_t x) {return x*9301+49297;}
static uint32_t p_random() {return seed = p_func(seed);}

// #define _DEBUG
int main() {
	int N, M, S, MOD;
	assert(scanf("%d %d %d %d", &N, &M, &S, &MOD) == 4);
	
	seed = S;
	
	uint32_t hash = 0;
#ifdef _DEBUG
	static int D[1048576];
#endif
	for (int prob_p = 0; prob_p <= 10; prob_p += 2) {
	for (int prob_q = 0; prob_q <=  4; prob_q += 1) {
	for (int limL = 4; limL <= 16; limL <<= 1) {
		struct timespec tt1, tt2;
		clock_gettime(CLOCK_REALTIME, &tt1); 
		for (int it = 0; it < M; it++) {
			init_ISMQ(N);
			uint32_t Base = 100000;
			for (int i = 0; i < N; i++) {
				// step 1: append value to array $A$
				{
					uint32_t V = 0;
					if (p_random()%10 < prob_p) {
						int diff = prob_q * 25 - (p_random()%100);
						if ((int) Base + diff < 0)
							diff = 1;
						Base += diff;
						V = Base;
						//					V = p_random()%MOD;
					}

					append_ISMQ(V);
#ifdef _DEBUG
					D[i] = V;
#endif
				}
				// step 2: query suffix maximum in $A[L, i]$
				for (int k = 0; k < 10; k++)
				{
					int L = min(max(i-(int) (p_random()%limL), 0), i);
					uint32_t ans = query_ISMQ(L);
					hash ^= ans;
#ifdef _DEBUG
					printf("max(A[%2d, %2d]) = %u\n", L, i, ans);
#endif
				}
			}
		}
		clock_gettime(CLOCK_REALTIME, &tt2);
		double diff_time = (tt2.tv_sec + 1e-9*tt2.tv_nsec) - 
							(tt1.tv_sec + 1e-9*tt1.tv_nsec);
		printf("p %3d q %3d L %2d time %.3f\n", prob_p * 10, prob_q * 25, limL, diff_time);
	}
	}
	}

#ifdef _DEBUG
	printf("[");
	for (int i = 0; i < N; i++)
		printf("%2u%c", D[i], i == N-1 ? ']' : ' ');
	puts("");
#endif

	printf("%X\n", hash);
	return 0;
}
/*
   16 1 17 20
 */

