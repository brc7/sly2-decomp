typedef unsigned long ulong;
struct S { char pad[0x40]; int f; char pad2[0x104]; ulong g; int pad3; int a; float b, c; char pad4[0x18]; int d; char pad5[8]; int e; };
void InitSo(struct S *p);
static inline int FIsPtr(int f) { return !(f & 1); }
void func_00229EF0(struct S *p)
{
    int f;
    InitSo(p);
    f = p->f;
    p->a = 50;
    p->b = 300.0f;
    p->c = 500.0f;
    p->d = -1;
    p->e = -1;
    if (FIsPtr(f)) *(int *)f |= 0xA;
    else p->f = f | 0xA;
    p->g &= ~(1L << 35);
}
