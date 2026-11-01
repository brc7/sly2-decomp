int CploFindSwObjectsByClass(void *, int, int, int, int, int *);
void SetStepguardSpecial(int, void *);
void func_0019F220(void *p, void *x) {
    int ap[32];
    int c, i;
    c = CploFindSwObjectsByClass(p, 0x105, 0x38, 0, 32, ap);
    for (i = 0; i < c; i++) SetStepguardSpecial(ap[i], x);
}
