#include "ISMQ.h"
#include <algorithm>
using namespace std;

int index;
uint32_t A[1048576];
void init_ISMQ(int N) {
	index = 0;
}
void append_ISMQ(uint32_t V) {
	A[index] = V;
	index++;
}
uint32_t query_ISMQ(uint32_t L) {
	uint32_t ret = 0;
	for (int i = L; i < index; i++)
		ret = max(ret, A[i]);
	return ret;
}

