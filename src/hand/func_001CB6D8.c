struct VTE { short d; short pad; void (*pf)(); };
struct LO { char pad[0xC]; struct VTE *vt; };
struct H { struct LO *plo; };
struct S { char pad[0x14]; struct H *ph; char pad2[0xE8 - 0x18]; void *pE8; };
struct SL;
extern struct SL D_003DD4BC;
void func_001CAA60(struct S *p, void *q);
int FFindDlEntry__FP2DLPv(struct SL *pl, void *pv);
void RemoveDlEntry__FP2DLPv(struct SL *pl, void *pv);
void func_001CB6D8(struct S *p)
{
    struct LO *plo;
    while (p->pE8) func_001CAA60(p, p->pE8);
    if (FFindDlEntry__FP2DLPv(&D_003DD4BC, p->ph)) {
        plo = p->ph->plo;
        ((void (*)(void *))plo->vt[22].pf)((char *)plo + plo->vt[22].d);
        RemoveDlEntry__FP2DLPv(&D_003DD4BC, p->ph);
    }
}
