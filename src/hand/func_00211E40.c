struct S { char pad[0xB0]; int f; };
void InitLo(struct S *p);
void func_00211E40(struct S *p)
{
    InitLo(p);
    p->f = 1;
}
