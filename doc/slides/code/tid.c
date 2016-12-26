int tid(int lrz, int lid, int rsz, int rid)
  if (rsz == 0)
    return lid;
  n = lsz+rsz+1
  base = 0
  for i from 0 to lsz-1
    base += Catalan[i]*Catalan[n-i-1]
  offset = lid*Catalan[rsz] + rid
  return base + offset