struct State {
  int i, s, tid
  <lsz,lid,value> D[s+1], Dp
}
State 
  init(<i=0, s=n, tid=C[n]-1, D[0].value=INF, Dp=0>)