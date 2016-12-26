LCA(int s)
  for n from 1 to s
    parallel for tid from 0 to Cn - 1
      parallel for p from 0 to n-1
        compute <lsz,lid,rsz,rid> in O(n)
        for q from p to n-1
          LCA[n][tid][p][q] = ...
  return 