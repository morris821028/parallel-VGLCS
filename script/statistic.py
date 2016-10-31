#!/usr/bin/python

import sys
import datetime

stat_time = dict()
stat_cycle = dict()
stat_count = dict()

for line in sys.stdin:
	token = line.split()
	stat_time[token[0]] = stat_time.get(token[0], 0.0) + float(token[3]);
	stat_cycle[token[0]] = stat_time.get(token[0], 0.0) + float(token[1]);
	stat_count[token[0]] = stat_count.get(token[0], 0) + 1;

today = datetime.date.today()

fout = open(str(today) + '.log', 'w+')

for key, value in stat_count.iteritems():
	fout.write(key + '\n')
	fout.write('\t%f seconds\n' % (stat_time[key] / value))
	fout.write('\t%f cycles\n' % (stat_cycle[key] / value))
#	print str(key)
#	print ('\t%f seconds' % (stat_time[key] / value))
#	print ('\t%f cycles' % (stat_cycle[key] / value))
