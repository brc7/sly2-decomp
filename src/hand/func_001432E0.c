extern void *D_003E1AC8;
int func_0013C478(void *p);
void func_00143380(void *p);
void *PloAcquireProxy(void *p, int n);
void func_0013C490(void *p, int n);
void func_0013C560(void *p, int n);
void func_001432E0(char *p, int n)
{
    void *q = *(void **)(p + 0x11A8);
    char *o;
    if (q && func_0013C478(q) == n) return;
    func_00143380(p);
    if (n == -1) return;
    if (!D_003E1AC8) return;
    o = PloAcquireProxy(D_003E1AC8, 1);
    func_0013C490(o, n);
    *(void **)(o + 0x160) = p;
    func_0013C560(o, 6);
    func_0013C560(o, 4);
}
