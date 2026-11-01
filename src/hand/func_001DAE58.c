struct E { int a[4]; int n; int b[3]; };
extern struct E D_002BCD00[];
int NEpisodeBest(void);
int func_001DAE58(int i) {
    struct E *pe = &D_002BCD00[i];
    if (pe->n == -1) return 0;
    return NEpisodeBest() >= pe->n;
}
