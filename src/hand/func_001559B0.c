struct S { int pad[4]; int n10; int pad2[7]; int n30; int pad3[2]; float g3C; };
extern float D_002A5134;
void InitBhv(struct S *p);
void func_00209890(int n, int k);
void func_001559B0(struct S *p)
{
    InitBhv(p);
    func_00209890(p->n10, 0x23);
    p->n30 = -1;
    p->g3C = D_002A5134;
}
