#include <bits/stdc++.h>
using namespace std;

int main(int argc, char* argv[]) {
	assert(argc == 4);
	fstream fin[3];
	for (int i = 0; i < 3; i++)
		fin[i].open(argv[i+1], fstream::in);

	map<int, map<int, map<int, double> > > R[3];
	for (int i = 0; i < 3; i++) {
		string foo;
		int p, q, L;
		double t;
		while (fin[i] >> foo >> p >> foo >> q >> foo >> L >> foo >> t) {
			R[i][p][q][L] = t;
		}
	}
	for (int p = 0; p <= 100; p += 20) {
		printf("$%d\\%$ ", p);
		double speedup = 0;
		for (int L = 4; L <= 16; L <<= 1) {
			for (int q = 0; q <= 100; q += 25) {
				printf("&");
				printf(" \\begin{tabular}{@{}r@{}}");
				char s1[32], s2[32], s3[32];
				int mnIdx = 0;
				double mnVal = 1e+30;
				for (int i = 0; i < 3; i++) {
					if (R[i][p][q][L] < mnVal)
						mnVal = R[i][p][q][L], mnIdx = i;
				}
				for (int i = 0; i < 3; i++) {
					if (mnIdx == i)
						printf(" \\textbf{%.2f}", R[i][p][q][L]);
					else
						printf("%.2f", R[i][p][q][L]);
					if (i != 2)
						printf("\\\\");
				}
				speedup = max(speedup, R[0][p][q][L] / R[2][p][q][L]);
//				printf("%.2f\\\\%.2f\\\\%.2f", R[0][p][q][L], R[1][p][q][L], R[2][p][q][L]);
				printf(" \\end{tabular}");
			}
		}
		if (speedup < 1)
			printf("& %.2f", speedup);
		else
			printf("& \\textbf{%.2f}", speedup);
		puts("\\\\ \\hline");
	}
	printf("Speedup ");
	for (int L = 4; L <= 16; L <<= 1) {
		for (int q = 0; q <= 100; q += 25) {
			double speedup = 0;
			for (int p = 0; p <= 100; p += 20) {
				speedup = max(speedup, R[0][p][q][L] / R[2][p][q][L]);
			}
			if (speedup < 1)
				printf("& %.2f", speedup);
			else
				printf("& \\textbf{%.2f}", speedup);

		}
	}
	printf("\\\\");
	return 0;
}
