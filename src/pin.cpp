#include <bits/stdc++.h>
#include <random>
using namespace std;
double frandom() {
	return (double)(rand()%10) / 2;
}
double mrandom() {
	return (rand() * rand())%10;
}
int main(int argc, char *argv[]) {
	assert(argc == 3 && "./pin <n: integer> <g: max gap>");
	int MAXN = atoi(argv[1]);
	int MAXG = atoi(argv[2]);
	fprintf(stderr, "MAXN = %d\n", MAXN);
    srand(time(NULL));
    int testcase = 10;
    while (testcase--) {
    	int n = MAXN, m = MAXN;
    	for (int i = 0; i < n; i++)
    		printf("%c", "ATCG"[rand()%4]);
    	puts("");
    	for (int i = 0; i < n; i++)
    		printf("%d%c", rand()%min(n, MAXG), " \n"[i==n-1]);
    	for (int i = 0; i < m; i++)
    		printf("%c", "ATCG"[rand()%4]);
    	puts("");
    	for (int i = 0; i < m; i++)
    		printf("%d%c", rand()%min(m, MAXG), " \n"[i==m-1]);
	}
    return 0;
}
