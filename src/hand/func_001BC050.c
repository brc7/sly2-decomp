struct S { char pad[0x1C4]; char dl[0x24]; int f1e8; };
void InitShape(struct S *p);
void InitDl__FP2DLi(void *pdl, int ib);
void func_001BC050(struct S *p)
{
    InitShape(p);
    p->f1e8 = -1;
    InitDl__FP2DLi(p->dl, 0x1D0);
}
