struct S { int a, b, c, d; int f10, f14, f18; };
extern char D_003D4A60[];
extern char D_002DE578[];
extern int D_002E1E00;
void *func_002333B4(void *pv, int n, int cb);
void func_001D8D80(void *p, int n);
void InitDifficulty(void *p);
void InitGame(struct S *p)
{
    func_002333B4(p, 0, sizeof(*p));
    p->f10 = -1;
    p->f14 = -1;
    p->f18 = -1;
    func_001D8D80(D_003D4A60, 0);
    InitDifficulty(D_002DE578);
    D_002E1E00 = 0;
}
