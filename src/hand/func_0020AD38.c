extern float D_002A88C8;
struct S { char pad[0x50]; int n50; float f54, f58, f5C, f60, f64, f68, f6C; char pad2[0x7C - 0x70]; int n7C; int n80; int n84; char pad3[0xA8 - 0x88]; int nA8; void *pAC; char eB0[4]; };
void InitLo(struct S *p);
void func_002090D0(void *pe, void *p);
void func_0020AD38(struct S *p)
{
    InitLo(p);
    p->n50 = 0;
    p->f54 = 300.0f;
    p->f58 = -5000.0f;
    p->f5C = 5000.0f;
    p->f60 = 0.2f;
    p->f64 = 0.5f;
    p->f68 = 0.17453295f;
    p->nA8 = 1;
    p->n7C = -1;
    p->n80 = -1;
    p->n84 = -1;
    p->f6C = D_002A88C8;
    func_002090D0(p->eB0, p->pAC);
}
