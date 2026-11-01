struct S { char pad[0x40]; int fl; };
void InitLo(struct S *p);
void SetLoBusyk(struct S *p, int n);
void func_0020B4A8(struct S *p) {
    int v;
    InitLo(p);
    v = p->fl;
    if ((v ^ 1) & 1) *(int *)v |= 2; else p->fl = v | 2;
    SetLoBusyk(p, 1);
}
