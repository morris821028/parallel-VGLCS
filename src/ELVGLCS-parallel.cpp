#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <algorithm>
#include <omp.h>
#include "VGLCS.h"
#include "helper.h"
#include "config.h"
#define MIN(x, y) ((x) < (y) ? (x) : (y))
#define MAX(x, y) ((x) > (y) ? (x) : (y))

/**
 * Elastic Variable Gapped Longest Common Subsequence
 *
 */

int serial_ELVGLCS(int nA, char A[], uint16_t GA[][2],
					int nB, char B[], uint16_t GB[][2]) {
	return 0;
}

int parallel_ELVGLCS(int nA, char A[], uint16_t GA[][2],
					int nB, char B[], uint16_t GB[][2]) {
	A--, B--, GA--, GB--;
	if (nA > nB) {
		std::swap(GA, GB), std::swap(nA, nB), std::swap(A, B);
	}
	assert(nA < MAXN && nB < MAXN);

	uint16_t ret = 0;
	uint16_t max_gapA = 0, max_gapB = 0;
	for (int i = 1; i <= nA; i++)
		max_gapA = MAX(max_gapA, GA[i][1]);
	max_gapA = MIN(max_gapA, nA);
	for (int i = 1; i <= nB; i++)
		max_gapB = MAX(max_gapB, GB[i][1]);
	max_gapB = MIN(max_gapB, nB);

	const int lognA = log2int(max_gapA+1);
	const int lognB = log2int(max_gapB+1);
	uint16_t *mem_tlbD = (uint16_t *) malloc((nA+1)*(lognA+1)*(nB+1)*sizeof(uint16_t));
	uint16_t *mem_tlbR = (uint16_t *) malloc((nB+1)*(lognB+1)*sizeof(uint16_t));
	assert(mem_tlbD != NULL && mem_tlbR != NULL);
	
	SparseTable Q[MAXN];
	SparseTable sp_tlb;
	sp_tlb.init(nB, lognB, mem_tlbR);

	const int P = 20;
	omp_set_num_threads(P);
	
	char logGA[MAXN], logGB[MAXN];
	for (int i = 1; i <= nA; i++) {
		int l = i - MIN(GA[i][1]+1, i), r = i - MIN(GA[i][0]+1, i);
		logGA[i] = log2int(r-l+1);
	}
	for (int i = 1; i <= nB; i++) {
		int l = i - MIN(GB[i][1]+1, i), r = i - MIN(GB[i][0]+1, i);
		logGB[i] = log2int(r-l+1);
	}
	
	#pragma omp parallel
	{
		// init sparse table
		#pragma omp for schedule(static)
		for (int i = 0; i <= nB; i++)
			Q[i].init(nA, lognA, mem_tlbD+(i*(nA+1)*(lognA+1)));

		for (int i = 1; i <= nA; i++) {
			// query: incremental suffix maximum query
			{
				int l = i - MIN(GA[i][1]+1, i), r = i - MIN(GA[i][0]+1, i);
				int t = logGA[i];
				uint16_t *tb = sp_tlb.tb[0];
				#pragma omp for schedule(static)
				for (int j = 1; j <= nB; j++) {
					tb[j] = Q[j].get(l, r, t);
				}
			}
				
			// doubling algorithm
			sp_tlb.parallel_build(nB, lognB);

			char Ai = A[i];
			// dynamic programming
			#pragma omp for schedule(static) reduction(max: ret)
			for (int j = 1; j <= nB; j++) {
				if (Ai == B[j]) {
					int l = j - MIN(GB[j][1]+1, j), r = j - MIN(GB[j][0]+1, j);
					uint16_t val = sp_tlb.get(l, r, logGB[j])+1;
					Q[j].set(i, val, lognA);
					ret = MAX(ret, val);
				} else {
					Q[j].set(i, 0, lognA);
				}
			}
		}
	}
	free(mem_tlbD);
	free(mem_tlbR);
	return ret;
}

#undef MIN
#undef MAX
