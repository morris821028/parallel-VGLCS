struct Node {
  int lsz, lid, val;
};
struct State {
  int i, s, tid, Dp;
  struct Node D[s+1];
};