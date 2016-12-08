#!/bin/bash

SUBTASK=subtasks.py.gen


#rm $SUBTASK
#
#echo -e '[\n' >>$SUBTASK
#for f in *.in
#do
#	tmp=${f%%.in}
#	echo -e " [10, ['${tmp}.in', '${tmp}.out', 60, 4000 << 20, 64 << 10]]," >>$SUBTASK
#done
#echo -e ']\n' >>$SUBTASK

#exit 0

RED='\033[0;31m'
NC='\033[0m' # No Color

for f in *.in
do
	tmp=${f%%.in}
	echo -e "${RED}testcase - $tmp${NC}\n"
	time -p ./cormq <$tmp.in >temp.out
	diff -w $tmp.out temp.out
done
