#include <bits/stdc++.h>
#include <random>
using namespace std;
double frandom() {
	return (double)(rand()%10) / 2;
}
double mrandom() {
	return (rand() * rand())%10;
}
int main() {
//	freopen("in.txt", "w", stdout);
    srand(time(NULL));
    int testcase = 10;
    while (testcase--) {
    	int N = 3000, M = 3000;
		N = M = 500;
    	for (int i = 0; i < N; i++)
			putchar("ATCG"[rand()%4]);
		puts("");
		for (int i = 0; i < N; i++)
			printf("%d ", rand()%(i+1));
		puts("");
    	for (int i = 0; i < M; i++)
			putchar("ATCG"[rand()%4]);
		puts("");
		for (int i = 0; i < M; i++) {
			printf("%d ", rand()%(i+1));
		}
		puts("");
	}
    return 0;
}
/*
GCGCAATG
3 1 1 3 0 0 2 1
GCCCTAGCG
2 0 3 2 0 1 2 0 1
*/
