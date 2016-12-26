#include <bits/stdc++.h>
#include <omp.h>
#include <sys/time.h>
using namespace std;

//
namespace {
	static const int MAXN = 8;
	static int Cn[MAXN], CnBase[MAXN][MAXN];
	struct T {
	char v[MAXN][MAXN];
	} _mem[1<<16];
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
				if (l < r)
				{
					int sum = 0;
					for (lsz = 0; CnBase[i][lsz] > l; lsz++);
					sum = CnBase[i][lsz];
					rsz = i-1-lsz;
					lid = (l - sum) / Cn[rsz];
					rid = (l - sum) % Cn[rsz];
				}
				// start
				T *store = LCA[i];
				for (int j = l; j < r; j++) {
					for (int p = 0; p < lsz; p++)
						for (int q = p; q < lsz; q++)
							store[j].v[p][q] = LCA[lsz][lid].v[p][q];
					for (int p = lsz+1; p < i; p++)
						for (int q = p; q < i; q++)
							store[j].v[p][q] = LCA[rsz][rid].v[p-lsz-1][q-lsz-1]+lsz+1;
					for (int p = 0; p <= lsz; p++)
						for (int q = lsz; q < i; q++)
							store[j].v[p][q] = lsz;
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
//		fprintf(stderr, "Run Success\n");
	}
	void ballotTlb() {
		Cn[0] = 1, Cn[1] = 1;
		CnBase[1][1] = 1;
		for (int i = 1; i < MAXN; i++) {
			int ret = 0;
			for (int j = 0; j < i; j++)
				CnBase[i][i-j-1] = ret, ret += Cn[j]*Cn[i-1-j];
			CnBase[i][i] = Cn[i] = ret;
		}
	}
	int tid(int lsz, int lid, int rsz, int rid) {
		if (rsz == 0)	return lid;
		int base = CnBase[lsz+rsz+1][lsz];
		int offset = rid + lid*Cn[rsz];
		return base + offset;
	}
	int typeOfCartesian(int A[], int s) {
		int D[s+1], Dp = 0;
		int I[s+1][2];
		D[0] = 0x3f3f3f3f;
		for (int i = 1; i <= s; i++) {
			int v = *A;
			int lsz = 0, lid = 0;
			while (D[Dp] < v) {
				lid = tid(I[Dp][0], I[Dp][1], lsz, lid);
				lsz += I[Dp][0]+1;
				Dp--;
			}
			Dp++, A++;
			D[Dp] = v;
			I[Dp][0] = lsz, I[Dp][1] = lid;
		}
		int lsz = 0, lid = 0;
		while (Dp) {
			lid = tid(I[Dp][0], I[Dp][1], lsz, lid);
			lsz += I[Dp][0]+1;
			Dp--;
		}
		return lid;
	}
	struct State {
		int i, s, tid;
		int D[MAXN+1][4], Dp;
    };
	int typeOfCartesian(State &state, int v) {
		int Dp = state.Dp;
		int lsz = 0, lid = 0;
		int bsz = state.s-state.i+1, bid = Cn[state.s-state.i+1]-1;
		while (state.D[Dp][0] < v) {
			lid = tid(state.D[Dp][1], state.D[Dp][2], lsz, lid);
			lsz += state.D[Dp][1]+1;
			bid = tid(state.D[Dp][1], state.D[Dp][2], bsz, bid);
			bsz += state.D[Dp][1]+1;
			Dp--;
		}
		Dp++;
		int ins = bid;
		int _tid = tid(lsz, lid, state.s-state.i, Cn[state.s-state.i]-1);
		// fprintf(stderr, "ins %d  ->  %d\n", ins, _tid);
		ins = _tid - ins;
		state.D[Dp][0] = v, state.D[Dp][1] = lsz, state.D[Dp][2] = lid, state.D[Dp][3] = _tid;
		state.i++;
		state.Dp = Dp;
		state.tid += ins;
		return state.tid;
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
	State t;
	t.i = 1, t.s = n, t.tid = Cn[n]-1;
	t.D[0][0] = 0x3f3f3f3f, t.Dp = 0;
	for (int i = 0; i < n; i++) {
		typeOfCartesian(t, A[i]);
		int c_tid = t.tid;
		for (int p = 0; p <= i; p++) {
			for (int q = p; q <= i; q++) {
				int mx = A[p];
				for (int k = p; k <= q; k++)
					mx = max(mx, A[k]);
				assert(mx == A[LCA[n][c_tid].v[p][q]]);
			}
		}
	}
	if (tid != t.tid) {
		for (int i = 0; i < n; i++)
			fprintf(stderr, "%d ", A[i]);
		fprintf(stderr, "\n");
		fprintf(stderr, "Expect %d, Coroutine %d\n", tid, t.tid);
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
/*
	for (int i = 0; i < 10000000; i++) {
		int A[MAXN] = {};
		int n = MAXN-1;
		for (int j = 0; j < n; j++)
			A[j] = rand()%n;
		if (test(A, n)) {
		} else {
			for (int k = 0; k < n; k++)
				printf("%d ", A[k]);
			puts("");
			assert(false && "Failed");
		}
	}
*/
	fprintf(stderr, "PASS ALL\n\n");
}
int nativeQuery(int A[], int n) {
	int sum = 0;
	for (int l = 0; l < n; l++) {
		for (int r = l; r < n; r++) {
			int mx = A[l];
			for (int k = l; k <= r; k++)
				mx = max(mx, A[k]);
			sum += mx;

		}
	}
	return sum;
}
int fastQuery(int A[], int n) {
	int tid = typeOfCartesian(A, n);
	int sum = 0;
	for (int l = 0; l < n; l++) {
		for (int r = l; r < n; r++) {
			sum += A[LCA[n][tid].v[l][r]];
		}
	}
	return sum;
}
void testExp() {
	// generate all permutations of input
	fprintf(stderr, "Compute Time\n");
	for (int i = 1; i < MAXN; i++) {
		fprintf(stderr, "#Node %d\n", i);
		{
			struct timeval t1, t2;
			double elapsedTime = 0;
			int avBase = 50;
			int checksum = 0;
			for (int it = 0; it < avBase; it++) {
				int A[MAXN] = {};
				for (int j = 0; j < i; j++)
					A[j] = j;
				int testcase = 0;
				do {
					gettimeofday(&t1, NULL);
					checksum += fastQuery(A, i);
					gettimeofday(&t2, NULL);
					elapsedTime += (t2.tv_sec - t1.tv_sec) * 1000.0;      // sec to ms
					elapsedTime += (t2.tv_usec - t1.tv_usec) / 1000.0;   // us to ms
				} while (next_permutation(A, A+i));

			}
			fprintf(stderr, "Fast Time %lf ms. Checksum %X\n", elapsedTime/avBase/(i*(i+1)/2), checksum);
		}
		{
			struct timeval t1, t2;
			double elapsedTime = 0;
			int avBase = 50;
			int checksum = 0;
			for (int it = 0; it < avBase; it++) {
				int A[MAXN] = {};
				for (int j = 0; j < i; j++)
					A[j] = j;
				int testcase = 0;
				do {
					gettimeofday(&t1, NULL);
					checksum += nativeQuery(A, i);
					gettimeofday(&t2, NULL);
					elapsedTime += (t2.tv_sec - t1.tv_sec) * 1000.0;      // sec to ms
					elapsedTime += (t2.tv_usec - t1.tv_usec) / 1000.0;   // us to ms
				} while (next_permutation(A, A+i));

			}
			fprintf(stderr, "Native Time %lf ms. Checksum %X\n", elapsedTime/avBase/(i*(i+1)/2), checksum);
		}

	}
	fprintf(stderr, "PASS ALL\n\n");
}

int main() {
	{
		struct timeval t1, t2;
		double elapsedTime;
		gettimeofday(&t1, NULL);
		ballotTlb();
		int avBase = 1000;
		for (int i = 0; i < avBase; i++)
			buildLCA();
		gettimeofday(&t2, NULL);

		// compute and print the elapsed time in millisec
		elapsedTime = (t2.tv_sec - t1.tv_sec) * 1000.0;      // sec to ms
		elapsedTime += (t2.tv_usec - t1.tv_usec) / 1000.0;   // us to ms
		fprintf(stderr, "Build Table Time %lf ms.\n", elapsedTime/avBase);

	}

	// testAll
	testAll();

	// expr
	testExp();

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

