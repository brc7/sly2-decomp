struct F { int a[0x1C]; };
struct S { char pad[0x108C]; int n; char ab[0xC]; };
void *func_002333B4(void *p, int c, int cb);
void *func_00205458(struct S *p, int oid);
void SetBhvguardMode(void *p, struct F *pf);
void func_001BA980(struct S *p, int n)
{
    struct F f;
    void *po;
    func_002333B4(p->ab, 0, 0xC);
    p->n = n;
    func_002333B4(&f, 0, sizeof(f));
    f.a[0] = 0xF;
    po = func_00205458(p, 0x9C);
    if (po) SetBhvguardMode(po, &f);
}
