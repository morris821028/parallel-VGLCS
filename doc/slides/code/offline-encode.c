int typeOfCartesian(int A[], int s)
  <lsz,lid,value> D[s+1]
  int Dp = 0
  D[0] = <0,0,INF>
  for i from 1 to s // build cartesian tree
    v = A[i], lsz = 0, lid = 0
    while D[Dp].value < v
       lid = tid(D[Dp].lsz, D[Dp].lid, lsz, lid)
       lsz += D[Dp].lsz+1, Dp--
    Dp++, D[Dp] = <lsz,lid,v>
  lsz = 0, lid = 0
  while Dp > 0 // pop all
  	lid = tid(D[Dp].lsz, D[Dp].lid, lsz, lid)
    lsz += D[Dp].lsz+1, Dp--
  return lid
