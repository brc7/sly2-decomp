struct S { char pad[0x40]; int f; char pad2[0xC]; float g; char pad3[8]; int h; char pad4[0x680]; int i; char pad5[0x20C]; char buf[0xA0]; };
void InitLo(struct S *p);
void func_0011A050(void *pv, int cb);
void SetLoBusyk(struct S *p, int a);
static inline int FIsPtr(int f) { return !(f & 1); }
void func_0022B720(struct S *p)
{
    int f;
    InitLo(p);
    p->h = -1;
    p->i = -1;
    p->g = -1.0f;
    func_0011A050(p->buf, 0xA0);
    SetLoBusyk(p, 1);
    f = p->f;
    if (FIsPtr(f)) *(int *)f |= 2;
    else p->f = f | 2;
}
