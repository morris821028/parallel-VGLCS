void demo(int l, int r, int S = 4) {
    const int POWS = 1<<S;
    // round up
    l = (l|(POWS-1))+1;
    // round down
    r = (r&(0x7FFFFFFF^(POWS-1)))-1;
}