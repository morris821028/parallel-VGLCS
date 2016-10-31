## Parallel VGLCS ##

Parallel Variable Gapped Longest Common Sequence (Parallel-VGLCS)

### Usage ###

```
#include "VGLCS.h"
...

#define MAXN 5005
int main() {
	char A[MAXN], B[MAXN];
    short GA[MAXN], GB[MAXN];
    int nA, nB;
	int lcs_len = serial_VGLCS(nA, A, GA, nB, B, GB);
	// parallel version
	// int lcs_len = parallel_VGLCS(nA, A, GA, nB, B, GB);
}
```

### Run Test ###

```
make
./unit
```
