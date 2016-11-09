## a374. 5. 股票趨勢 ##

Fixed Gapped Longest Common Sequence

Given two sequences of a string X and Y, and the gap constraint corresponding different kind of character.

### Definition ###

* n: the length of string X 
* m: the length of string Y
* r: the maximum distance of gap constraint
* |r|: the number of different kind of gap

### Algorithm ###

* basic brute force: O(n m r^2) 
* using range tree: O(n m log^2 n)
* using monotone queue: O(n m |r|)
* using disjoint set: O(alpha n m) 

### Update ###

Rank 1 Accpeted(52ms, 4MB) on Zerojudge 2016-11-08 19:52
