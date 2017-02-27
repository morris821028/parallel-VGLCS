#include <bits/stdc++.h>
using namespace std;

const int MAXN = 10005;

struct ISMQ {
	int16_t value[MAXN];
	int16_t parent[MAXN];
	int16_t weight[MAXN];
	int16_t leader[MAXN];
	int16_t lIdx;
	int findp(int x) {
        return parent[x] == x ? x : (parent[x] = findp(parent[x]));
    }
	void init(int n) {
		for (int i = 0; i <= n; i++)
			parent[i] = i;
		for (int i = 0; i <= n; i++)
			weight[i] = 1;
		memset(value, 0, sizeof(value[0])*(n+1));

        lIdx = 0;
        value[0] = 0, parent[0] = 0, weight[0] = 1;
        leader[lIdx] = 0;
	}
	int get(int x) {
        return value[findp(x)];
    }
    void append(int x, int16_t val) {
        value[x] = val, parent[x] = x;
        int u = x, weightR = 1;
        for (int16_t *v = leader + lIdx; lIdx >= 0 && value[*v] <= val; v--, lIdx--) {
            if (weightR <= weight[lIdx])
                u = (parent[u] = *v);
            else
                parent[*v] = u;
            weightR = weight[lIdx] + weightR;
        }
        ++lIdx;
        value[u] = val;
        leader[lIdx] = u;
        weight[lIdx] = weightR;
    }
};
int run(int nA, char A[], int16_t GA[], 
		int nB, char B[], int16_t GB[]) {
	A--, B--, GA--, GB--;
	if (nA > nB) {
		swap(GA, GB), swap(nA, nB), swap(A, B);
	}
	static ISMQ Q[MAXN];
	
	for (int i = 0; i <= nB; i++)
		Q[i].init(nA);
		
	int16_t ret = 0;
	for (int i = 1; i <= nA; i++) {
		int r = i - min(GA[i]+1, i);
		static ISMQ RQ;
		RQ.init(nB);
		for (int j = 1; j <= nB; j++) {
			int16_t tmp = 0;
			if (A[i] == B[j]) {
				tmp = RQ.get(j - min(GB[j]+1, j))+1;
				ret = max(ret, tmp);
			}
			int16_t Qr = Q[j].get(r);
			RQ.append(j, Qr);
			Q[j].append(i, tmp);
		}
	}
	return ret;
}

int main() {
	static char A[MAXN], B[MAXN];
	static int16_t GA[MAXN], GB[MAXN];
	int nA, nB, Q;
	while (scanf("%s", A) == 1) {
		nA = strlen(A);
		for (int i = 0; i < nA; i++)
			scanf("%hd", &GA[i]);
		scanf("%s", B);
		nB = strlen(B);
		for (int i = 0; i < nB; i++)
			scanf("%hd", &GB[i]);
		int ret = run(nA, A, GA, nB, B, GB);
		printf("%d\n", ret);
	}
	return 0;
}



