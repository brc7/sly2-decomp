int CploFindSwObjectsByClass(void *, int, int, int, int, int *);
void func_00207030(int, void *);
void func_0019F220(void *p, void *x) {
    int ap[32];
    int c, i;
    c = CploFindSwObjectsByClass(p, 0x105, 0x38, 0, 32, ap);
    for (i = 0; i < c; i++) func_00207030(ap[i], x);
}
