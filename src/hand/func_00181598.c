struct S { char pad[0x200]; int a; int b; };
extern char D_003DD490[];
void func_0017F6B0(struct S *p);
void func_001815E8(struct S *p, int n);
void TriggerRsmg(void *g, int a, int *pb, struct S *p, int n);
void func_00181598(struct S *p)
{
    func_0017F6B0(p);
    func_001815E8(p, 0);
    TriggerRsmg(D_003DD490, p->a, &p->b, p, 0);
}
