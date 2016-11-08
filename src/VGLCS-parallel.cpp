#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <algorithm>
#include <omp.h>
#include "VGLCS.h"
#include "config.h"
#include "helper.h"
#define MIN(x, y) ((x) < (y) ? (x) : (y))
#define MAX(x, y) ((x) > (y) ? (x) : (y))

/**
 * Variable Gapped Longest Common Subsequence
 *
 */

int serial_VGLCS(int nA, char A[], int16_t GA[],
        int nB, char B[], int16_t GB[]) {
    A--, B--, GA--, GB--;
	assert(nA < MAXN);

	int16_t *mem_base = (int16_t *) malloc(sizeof(int16_t)*(nA+1)*(nB+1)*3);
	int16_t *mem_ismq = (int16_t *) malloc(sizeof(int16_t)*(nB+1)*3);
	assert(mem_base != NULL);
	assert(mem_ismq != NULL);
    ISMQ Q[MAXN];
    for (int i = 0; i <= nB; i++)
        Q[i].init(nA, mem_base+(i*(nA+1)*3));

    int16_t ret = 0;
    for (int i = 1; i <= nA; i++) {
        int r = i - MIN(GA[i]+1, i);
        ISMQ RQ;
        RQ.init(nB, mem_ismq);
        for (int j = 1; j <= nB; j++) {
			int16_t tmp = 0;
            if (A[i] == B[j]) {
                tmp = RQ.get(j - MIN(GB[j]+1, j))+1;
            	ret = MAX(ret, tmp);
            }
            RQ.set(j, Q[j].get(r));
			if (tmp)
	            Q[j].set(i, tmp);
        }
    }
	free(mem_base);
	free(mem_ismq);
    return ret;
}

int parallel_VGLCS(int nA, char A[], int16_t GA[], 
					int nB, char B[], int16_t GB[]) {
	A--, B--, GA--, GB--;
	if (nA > nB) {
		std::swap(GA, GB), std::swap(nA, nB), std::swap(A, B);
	}
	assert(nA < MAXN && nB < MAXN);

	ISMQ Q[MAXN];
	char logGB[MAXN];
	int16_t ret = 0, max_gap = 0;

	{
		for (int i = 1; i <= nB; i++)
			max_gap = MAX(max_gap, GB[i]);

		max_gap = MIN(max_gap, nB);

		for (int i = 1; i <= nB; i++) {
			int l = i - MIN(GB[i]+1, i), r = i-1;
			logGB[i] = log2int(r-l+1);
		}
	}

	const int P = 20;
	const int lognB = log2int(max_gap+1);
	int16_t *mem_tlbD = (int16_t *) malloc(sizeof(int16_t)*(nA+1)*3*(nB+1));
	int16_t *mem_tlbR = (int16_t *) calloc((nB+1)*(lognB+1), sizeof(int16_t));	
	SparseTable sp_tlb;
	
	{
		assert(mem_tlbD != NULL);
		assert(mem_tlbR != NULL);
		sp_tlb.init(nB, lognB, mem_tlbR);
		omp_set_num_threads(P);
	}

	#pragma omp parallel
	{	
		// init ISMQ
		#pragma omp for schedule(static)
		for (int i = 0; i <= nB; i++)
			Q[i].init(nA, mem_tlbD+(i*(nA+1)*3));

		for (int i = 1; i <= nA; i++) {
			// query: incremental suffix maximum query
			{
				int r = i - MIN(GA[i]+1, i);
				int16_t *tb = sp_tlb.tb[0];
				#pragma omp for schedule(static)
				for (int j = 1; j <= nB; j++)
					tb[j] = Q[j].get(r);
			}
				
			// doubling algorithm
			sp_tlb.parallel_build(nB, lognB);

			char Ai = A[i];
			// dynamic programming
			#pragma omp for schedule(static) reduction(max: ret)
			for (int j = 1; j <= nB; j++) {
				if (Ai == B[j]) {
					int l = j - MIN(GB[j]+1, j), r = j-1;
					int16_t val = sp_tlb.get(l, r, logGB[j])+1;
					Q[j].set(i, val);
					ret = MAX(ret, val);
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
