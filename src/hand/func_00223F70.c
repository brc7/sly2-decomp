struct S { char pad[0x40]; int f; char pad2[0x10C]; int a, b, c, d; int pad3[4]; float e, g; int h; };
void InitSo(struct S *p);
static inline int FIsPtr(int f) { return !(f & 1); }
void func_00223F70(struct S *p)
{
    int f;
    InitSo(p);
    f = p->f;
    p->h = -10;
    p->e = 3000.0f;
    p->c = -1;
    p->a = -1;
    if (FIsPtr(f)) *(int *)f |= 6;
    else p->f = f | 6;
}
