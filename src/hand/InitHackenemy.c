struct S { char pad[0x1C0]; int x1c0; char p0[0x5C]; char x220[0x118]; float x338; int p1; float x340; char p2[0xC]; float x350; char p3[0x10]; float x364; char p4[8]; float x370; float x374; };
void func_0017F090(struct S *p);
void func_001813C0(struct S *p, float g);
void func_001813F0(struct S *p, float g);
void func_0017F3E0(struct S *p, int n, void *pv);
void InitHackenemy(struct S *p)
{
    func_0017F090(p);
    p->x340 = 1.0f;
    p->x350 = 1.0f;
    p->x364 = 1.0f;
    p->x338 = 250.0f;
    p->x370 = -3.40282346638528859812e+38f;
    p->x374 = 3.40282346638528859812e+38f;
    func_001813C0(p, 1.0f);
    func_001813F0(p, 1.0f);
    func_0017F3E0(p, 5, p->x220);
    p->x1c0 = -1;
}
