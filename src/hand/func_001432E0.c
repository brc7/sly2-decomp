extern void *D_003E1AC8;
int IdBootyCur(void *p);
void func_00143380(void *p);
void *PloAcquireProxy(void *p, int n);
void SelectBootyEntry(void *p, int n);
void SetBootyState(void *p, int n);
void func_001432E0(char *p, int n)
{
    void *q = *(void **)(p + 0x11A8);
    char *o;
    if (q && IdBootyCur(q) == n) return;
    func_00143380(p);
    if (n == -1) return;
    if (!D_003E1AC8) return;
    o = PloAcquireProxy(D_003E1AC8, 1);
    SelectBootyEntry(o, n);
    *(void **)(o + 0x160) = p;
    SetBootyState(o, 6);
    SetBootyState(o, 4);
}
