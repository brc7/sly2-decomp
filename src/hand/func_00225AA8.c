struct S { char pad[0x1CC]; int x; };
void PostSoLoad(struct S *p);
void func_00226370(struct S *p, int a);
void func_00225AA8(struct S *p)
{
    PostSoLoad(p);
    if (p->x) { p->x = 0; func_00226370(p, 1); }
}
