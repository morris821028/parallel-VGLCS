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
    int testcase = 1;
    while (testcase--) {
    	char sA[5005], sB[5005];
    	int nA = rand()%4000+1000, nB = rand()%4000+1000;
    	for (int i = 0; i < nA; i++)
    		sA[i] = rand()%26+ 'A';
    	sA[nA] = '\0';
    	for (int i = 0; i < nB; i++)
    		sB[i] = rand()%26 + 'A';
    	sB[nB] = '\0';
    	printf("%s %s\n", sA, sB);
    	
    	int Q = 5;
    	printf("%d\n", Q);
    	for (int i = 0; i < Q; i++) {
    		int dist[128] = {};
    		memset(dist, -1, sizeof(dist));
    		int used = rand()%26+1;
    		for (int j = 0; j < used; j++) {
    			int c = rand()%26 + 'A';
    			if (dist[c] != -1)
    				continue;
    			if (rand()%2)
    				dist[c] = rand()%5000;
    			else
    				dist[c] = rand()%50;
    			printf("%c %d ", c, dist[c]);
			}
			puts("$");
		}
	}
    return 0;
}
