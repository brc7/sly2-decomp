int func_00210A88(void);
int FMatchLoadFilter__FiPs(int c, short *a) {
    int x = func_00210A88();
    int i;
    short *ps;
    if (x == -1) return 1; for (i = 0; ; i++) { if (i >= c) return 0; if (a[i] == x) break; } return 1;
}
