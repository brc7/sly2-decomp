struct S { char pad[0x110]; int a; int b; int c; int d; int e; };
extern struct S *D_003E1C04;
void InitLo(struct S *p);
void func_0015CD68(struct S *p)
{
    InitLo(p);
    p->a = -1;
    p->b = -1;
    p->d = -1;
    p->e = -1;
    D_003E1C04 = p;
}
