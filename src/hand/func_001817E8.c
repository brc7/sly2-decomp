struct S { char pad[0x40]; int fl; char p2[0x248]; char dl[0xC]; };
void func_0017F090(struct S *p);
void InitDl__FP2DLi(void *pdl, int ib);
void func_001817E8(struct S *p)
{
    int v;
    func_0017F090(p);
    v = p->fl;
    if ((v ^ 1) & 1) *(int *)v |= 8;
    else p->fl = v | 8;
    InitDl__FP2DLi(p->dl, 0x60);
}
