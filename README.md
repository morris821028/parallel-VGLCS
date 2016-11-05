## Parallel VGLCS ##

Parallel Variable Gapped Longest Common Sequence (Parallel-VGLCS)

### Usage ###

```cpp
#include "VGLCS.h"
...

int main() {
...

#define MAXN 5005
	char A[MAXN], B[MAXN];
    short GA[MAXN], GB[MAXN];
    int nA, nB;
	int lcs_len = serial_VGLCS(nA, A, GA, nB, B, GB);
	// parallel version
	// int lcs_len = parallel_VGLCS(nA, A, GA, nB, B, GB);

...
}
```

### Run Test ###

```bash
make
./unit
```

### Run in Debug Mode ###

```bash
make DEBUG=1
./unit
```
