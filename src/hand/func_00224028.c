struct S { char pad[0x164]; void *x; char y[4]; };
void AddSoHierarchy(struct S *p);
void ReplaceSma(void *a, struct S *p, int b, int c, void *d);
void func_00224028(struct S *p)
{
    AddSoHierarchy(p);
    if (p->x) ReplaceSma(p->x, p, -1, 0, p->y);
}
