int appendAndTypeOfCartesian(State &state, int v)
  Dp = state.Dp, lsz = lid = 0
  bsz = state.s-state.i+1
  bid = C[bsz]-1
  while state.D[Dp]..value < v
    lid = tid(D[Dp].lsz, D[Dp].lid, lsz, lid)
    bid = tid(D[Dp].lsz, D[Dp].lid, bsz, bid)
    lsz += D[Dp].lsz+1, bsz += D[Dp].lsz+1
    Dp--
  Dp++
  state.D[Dp] = <lsz,lid,v>, state.Dp = Dp
  state.tid += bid - tid(lsz,lid,state.s-state.i,C[state.s-state.i]-1)
  state.i++
  return state.tid