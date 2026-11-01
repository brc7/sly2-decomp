struct S { char pad[0x15C]; int f; };
void AddSoHierarchy(struct S *p);
void func_0021BC08(struct S *p);
void func_0021BBD0(struct S *p)
{
    AddSoHierarchy(p);
    if (p->f)
        func_0021BC08(p);
}
