typedef int qword __attribute__((mode(TI)));
struct S { char pad[0x1880]; int n; char p1[0xC]; qword aq[3]; char p2[0xEC]; float g; };
void UpdateSoTreeBounds(struct S *p);
void ExtendSoTreeBounds(struct S *p, qword q, float g);
void func_0018A2E0(struct S *p)
{
    int i;
    UpdateSoTreeBounds(p);
    for (i = 0; i < p->n; i++)
        ExtendSoTreeBounds(p, p->aq[i], p->g);
}
