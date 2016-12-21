#include <bits/stdc++.h>
#include <omp.h>
using namespace std;

//
namespace {
	static const int MAXN = 13;
	static int C[MAXN][MAXN] = {}, Cn[MAXN];
	struct T {
		char v[MAXN][MAXN];
	} _mem[1<<20];
	static T *LCA[MAXN];
	void printTidInfo(int l, int i) {
		int lsz, rsz, lid, rid;
		{
			int sum = 0;
			for (rsz = 0; sum + Cn[rsz]*Cn[i-1-rsz] <= l; rsz++)
				sum += Cn[rsz]*Cn[i-1-rsz];
			lsz = i-1-rsz;
			lid = (l - sum) / Cn[rsz];
			rid = (l - sum) % Cn[rsz];
		}
		fprintf(stderr, "size[%2d], id[%2d], subid<%2d %2d>, sub<%2d %2d>\n", i, l, lid, rid, lsz, rsz);
		
		for (int p = 0; p < i; p++) {
			for (int q = 0; q < i; q++)
				fprintf(stderr, "%2d ", LCA[i][l].v[p][q]);
			fputs("\n", stderr);
		}
	}
	void buildLCA() {
		const int P = 4;
		omp_set_num_threads(P);
		LCA[0] = _mem;
		LCA[1] = _mem+1;
		for (int i = 2, off = 1; i < MAXN; i++) {
			off += Cn[i-1];
			LCA[i] = _mem + off;
			int chunk = (Cn[i]+P-1)/P;
			// for (int j = 0; j < Cn[i]; j++)
			#pragma omp parallel firstprivate(i)
			{
				// prepare
				int tid = omp_get_thread_num();
				int l = tid * chunk;
				int r = min(l + chunk, Cn[i]);
				int lsz, rsz, lid, rid;
//				#pragma omp critical
//				fprintf(stderr, "[%d][%d] %d %d, %d\n", i, tid, l, r, Cn[i]);
				if (l < r)
				{
					int sum = 0;
					for (rsz = 0; sum+Cn[rsz]*Cn[i-1-rsz] <= l; rsz++)
						sum += Cn[rsz]*Cn[i-1-rsz];
					lsz = i-1-rsz;
					lid = (l - sum) / Cn[rsz];
					rid = (l - sum) % Cn[rsz];
				}
				// start
				for (int j = l; j < r; j++) {
					for (int p = 0; p < lsz; p++)
						for (int q = p; q < lsz; q++)
							LCA[i][j].v[p][q] = LCA[lsz][lid].v[p][q];
					for (int p = lsz+1; p < i; p++)
						for (int q = p; q < i; q++)
							LCA[i][j].v[p][q] = LCA[rsz][rid].v[p-lsz-1][q-lsz-1]+lsz+1;
					for (int p = 0; p <= lsz; p++)
						for (int q = lsz; q < i; q++)
							LCA[i][j].v[p][q] = lsz;
//					printTidInfo(j, i);
					rid++;
					if (rid == Cn[rsz]) {
						rid = 0, lid++;
						if (lid == Cn[lsz])
							lid = 0, lsz--, rsz++;
					}
				}
			}
		}
		fprintf(stderr, "Run Success\n");
	}
	void ballotTlb() {
		C[0][0] = 1;
		for (int i = 0; i < MAXN; i++) {
			for (int j = 0; j < MAXN; j++) {
				if (i <= j && j)
					C[i][j] = C[i][j-1] + (i ? C[i-1][j] : 0);
			}
			Cn[i] = C[i][i];
		}
	}
	int _tid(int lsz, int lid, int rsz, int rid) {
		if (rsz == 0)	return lid;
		int sz = lsz+rsz;
		int tid = 0;
		for (int i = 0; i < rsz; i++)
			tid += Cn[i]*Cn[sz-i];
		return tid + rid + lid*Cn[rsz];
	}
	int typeOfCartesian(int A[], int s) {
		int D[s+1], Dp = 0;
		int I[s+1][2];
		D[0] = 0x3f3f3f3f;
		for (int i = 1; i <= s; i++) {
			int v = *A;
			int lsz = 0, lid = 0;
			while (D[Dp] < v) {
				lid = _tid(I[Dp][0], I[Dp][1], lsz, lid);
				lsz += I[Dp][0]+1;
				Dp--;
			}
			Dp++, A++;
			D[Dp] = v;
			I[Dp][0] = lsz, I[Dp][1] = lid;
		}
		int lsz = 0, lid = 0;
		while (Dp) {
			lid = _tid(I[Dp][0], I[Dp][1], lsz, lid);
			lsz += I[Dp][0]+1;
			Dp--;
		}
		return lid;
	}
}

int test(int A[], int n) {
	int tid = typeOfCartesian(A, n);
	for (int l = 0; l < n; l++) {
		for (int r = l; r < n; r++) {
			int mx = A[l];
			for (int k = l; k <= r; k++)
				mx = max(mx, A[k]);
			if (mx != A[LCA[n][tid].v[l][r]])
				return 0;
		}
	}
	return 1;
}
void testAll() {
	fprintf(stderr, "Run Test All\n");
	// generate all permutations of input
	for (int i = 1; i < MAXN; i++) {
		int A[MAXN] = {};
		for (int j = 0; j < i; j++)
			A[j] = j;
		int testcase = 0;
		fprintf(stderr, "#Node %d\n", i);
		do {
			if (test(A, i)) {
//				if (testcase % 1000 == 0)
//				printf("n[%d] Testcase #%3d: PASS\n", i, ++testcase);
			} else {
				for (int k = 0; k < i; k++)
					printf("%d ", A[k]);
				puts("");
				assert(false && "Failed");
			}
		} while (next_permutation(A, A+i));
	}
	for (int i = 0; i < 10000000; i++) {
		int A[MAXN] = {};
		int n = MAXN-1;
		for (int j = 0; j < n; j++)
			A[j] = rand()%n;
		if (test(A, n)) {
//			if ((i+1) % 1000 == 0)
//			printf("n[%d] Rand Testcase #%3d: PASS\n", n, i+1);
		} else {
			for (int k = 0; k < n; k++)
				printf("%d ", A[k]);
			puts("");
			assert(false && "Failed");
		}
	}
}
int main() {
	ballotTlb();
	buildLCA();
	
	// testAll
//	testAll();
	
//	char line[128];
//	while (fgets(line, 100, stdin)) {
//		stringstream sin(line);
//		int n = 0, x;
//		int A[16] = {0};
//		while (sin >> x)
//			A[n++] = x;
//		
//		int tid = typeOfCartesian(A, n);
//		printf("tid = %d\n", tid);
//		printTidInfo(tid, n);
//		for (int l = 0; l < n; l++) {
//			for (int r = l; r < n; r++) {
//				int mx = A[l];
//				for (int k = l; k <= r; k++)
//					mx = max(mx, A[k]);
//				if (mx != A[LCA[n][tid].v[l][r]])
//				printf("max(A[%d, %d]) = %d\n", l, r, mx);
//			}
//		}
//	}
	return 0;
}

