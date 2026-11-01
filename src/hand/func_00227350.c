struct S { char pad[0x110]; int x; };
void InitXfm(struct S *p);
void func_00227350(struct S *p)
{
    InitXfm(p);
    p->x = -1;
}
