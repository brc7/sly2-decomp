struct S { int pad[4]; int n10; int pad2[7]; int n30; };
void InitBhv(struct S *p);
void func_00209890(int n, int k);
void func_00155560(struct S *p)
{
    InitBhv(p);
    func_00209890(p->n10, 0x22);
    p->n30 = -1;
}
