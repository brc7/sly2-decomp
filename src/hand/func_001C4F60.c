struct VTE { short d; short pad; void (*pf)(); };
struct LO { char pad[0xC]; struct VTE *vt; };
struct S {
    char pad[0xC]; struct VTE *vt;
    char p0[0x158 - 0x10]; int x158;
    char p1[0x16C - 0x15C]; struct LO *x16C;
    char p2[0x1A4 - 0x170]; int x1A4; void *x1A8; int x1AC; void *x1B0; int x1B4; void *x1B8;
    char p3[0x204 - 0x1BC]; int x204; void *x208; int x20C; void *x210; int x214; void *x218;
};
extern char D_003DD490[];
void PostSoLoad(struct S *p);
struct LO *PloFindSwObjectByClass__FP2SWi3CIDP2LO(void *pg, int a, int b, struct S *p);
void *PloFindSwObject__FP2SWi3OIDP2LO(void *pg, int a, int b, struct S *p);
void func_001C4F60(struct S *p)
{
    struct LO *plo;
    PostSoLoad(p);
    plo = PloFindSwObjectByClass__FP2SWi3CIDP2LO(D_003DD490, 1, 0x12, p);
    p->x16C = plo;
    if (plo)
        ((void (*)(void *))plo->vt[9].pf)((char *)plo + plo->vt[9].d);
    if (p->x1A4 != -1) p->x1A8 = PloFindSwObject__FP2SWi3OIDP2LO(D_003DD490, 4, p->x1A4, p);
    if (p->x1AC != -1) p->x1B0 = PloFindSwObject__FP2SWi3OIDP2LO(D_003DD490, 4, p->x1AC, p);
    if (p->x1B4 != -1) p->x1B8 = PloFindSwObject__FP2SWi3OIDP2LO(D_003DD490, 4, p->x1B4, p);
    if (p->x204 != -1) p->x208 = PloFindSwObject__FP2SWi3OIDP2LO(D_003DD490, 4, p->x204, p);
    if (p->x20C != -1) p->x210 = PloFindSwObject__FP2SWi3OIDP2LO(D_003DD490, 4, p->x20C, p);
    if (p->x214 != -1) p->x218 = PloFindSwObject__FP2SWi3OIDP2LO(D_003DD490, 4, p->x214, p);
    ((void (*)(void *, int))p->vt[76].pf)((char *)p + p->vt[76].d, p->x158);
}
