void append(int tb[][MAXN], int pos) {
  // origin
  for (int i = 1; pos; i++) {
      int16_t p = tb[i-1][pos - (1<<i-1)];
      int16_t q = tb[i-1][pos];
      tb[i][pos] = MAX(q, p);
  }
  // better
  for (int i = 1, before = pos-1; before >= 0; i++) {
      int16_t p = tb[i-1][before];
      int16_t q = tb[i-1][pos];
      tb[i][pos] = MAX(q, p);
      before -= 1<<(i-1);
  }
}