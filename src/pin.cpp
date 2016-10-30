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
    int N;
    int testcase = 10;
    while (testcase--) {
    	int n = 5000, m = 5000;
    	for (int i = 0; i < n; i++)
    		printf("%c", "ATCG"[rand()%4]);
    	puts("");
    	for (int i = 0; i < n; i++)
    		printf("%d%c", rand()%10, " \n"[i==n-1]);
    	for (int i = 0; i < m; i++)
    		printf("%c", "ATCG"[rand()%4]);
    	puts("");
    	for (int i = 0; i < m; i++)
    		printf("%d%c", rand()%10, " \n"[i==m-1]);
	}
    return 0;
}
