struct S { char pad[0x17C]; int n; };
void CloneXfm(struct S *p);
void func_001E7840(struct S *p, int n);
void CloneShape(struct S *p)
{
    CloneXfm(p);
    if (p->n) {
        p->n = 0;
        func_001E7840(p, 0);
    }
}
