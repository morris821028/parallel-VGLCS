#!/bin/bash

SUBTASK=subtasks.py.gen

#rm $SUBTASK

#echo -e '[\n' >>$SUBTASK
#for f in *.in
#do
#	tmp=${f%%.in}
#	echo -e " [10, ['${tmp}.in', '${tmp}.out', 60, 4000 << 20, 64 << 10]]," >>$SUBTASK
#done
#echo -e ']\n' >>$SUBTASK

#exit 0

for f in *.in
do
	tmp=${f%%.in}
	echo "testcase - $tmp"
	time -p ./ISMQ-disjoint <$tmp.in >$tmp.out
done
