extern float D_002A8610;
extern int D_002C3CF0;
struct F { char pad[0xF0]; int a; int b; };
struct S {
    char p0[0x2DC]; int a2DC[3]; int n2E8; char p1[0x330 - 0x2EC]; int n330; char p2[0x360 - 0x334]; float f360;
    char p3[0x390 - 0x364]; int n390; char p4[0x484 - 0x394]; int n484, n488, n48C, n490; char p5[0x4A0 - 0x494]; int n4A0;
    char p6[0xE94 - 0x4A4]; int nE94; char p7[0xF64 - 0xE98]; struct F *pf; int nF68; char p8[0xF94 - 0xF6C]; int nF94;
    char p9[0xFA8 - 0xF98]; int nFA8; char p10[0xFC8 - 0xFAC]; int nFC8, nFCC; char p11[0xFF4 - 0xFD0]; float fFF4;
    char p12[0x1000 - 0xFF8]; int n1000;
};
void func_001CB5D0(struct S *p);
void func_0020B210(struct S *p);
void *func_002333B4(void *p, int c, int n);
void SetStepguardTarget(struct S *p, int n);
void func_002088E0(struct S *p);
void func_001F17F8(struct S *p, int a);
void SetStepguardSpecial(struct S *p, int n);
void ClearStepguardState(struct S *p)
{
    int one;
    func_001CB5D0(p);
    func_0020B210(p);
    p->n2E8 = 0;
    func_002333B4(p->a2DC, 0, 0xC);
    p->nE94 = 0;
    p->pf->a = 0;
    p->pf->b = 0;
    p->n484 = 0;
    p->n488 = 0;
    p->n48C = 0;
    p->n490 = 0;
    p->nF94 = 0;
    p->nFA8 = 0;
    p->n390 = 0;
    SetStepguardTarget(p, 0);
    func_002088E0(p);
    one = 1;
    p->n330 = 0;
    p->nF68 = 0;
    p->nFC8 = 0;
    p->nFCC = 0;
    p->f360 = -10.0f;
    p->n4A0 = one;
    p->fFF4 = D_002A8610;
    p->n1000 = D_002C3CF0;
    func_001F17F8(p, -1);
    SetStepguardSpecial(p, -1);
}
