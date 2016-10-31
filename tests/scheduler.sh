#!/bin/bash

INPUT=5.in
OUTPUT=5.out

export KMP_AFFINITY=granularity=fine,compact
echo $KMP_AFFINITY

time -p ./VGLCS-parallel <$INPUT >tmp.out
diff tmp.out $OUTPUT

export KMP_AFFINITY=granularity=fine,scatter
echo $KMP_AFFINITY

time -p ./VGLCS-parallel <$INPUT >tmp.out
diff tmp.out $OUTPUT

export KMP_AFFINITY=granularity=fine,compact,0,3
echo $KMP_AFFINITY

time -p ./VGLCS-parallel <$INPUT >tmp.out
diff tmp.out $OUTPUT


rm tmp.out
