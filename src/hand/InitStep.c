struct S { char pad[0x200]; float g200; char p1[0x10]; float g214; char p2[8]; float g220; char p3[0x3C]; float g260; int n264; char p4[4]; char x26C[4]; };
void InitPo(struct S *p);
void func_00204D48(struct S *p, float g);
void func_00204660(struct S *p, float g);
void InitBhvstk(void *pv, struct S *p);
void InitStep(struct S *p)
{
    InitPo(p);
    p->n264 = -1;
    p->g260 = 1.0f;
    p->g200 = 1.0f;
    func_00204D48(p, 1.0f);
    func_00204660(p, 0.0f);
    p->g220 = 30.0f;
    p->g214 = -50.0f;
    InitBhvstk(p->x26C, p);
}
