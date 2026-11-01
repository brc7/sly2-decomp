struct VTE { short d; short pad; void (*pf)(); };
struct LO { char pad[0xC]; struct VTE *vt; };
struct S { char pad[0x150]; int x150; struct LO *x154; struct LO *x158; };
int FIsBasicDerivedFrom__FP5BASIC3CID(struct LO *plo, int n);
struct LO *PloAcquireProxy(struct LO *plo, int n);
void func_001C7A58(struct LO *plo, int id);
int PxfFindSo(struct S *p, void *pv, int n);
int func_001C2DC8(struct S *p, struct LO *plo, int id, void *pv)
{
    if (p->x158) return 0;
    p->x154 = 0;
    if (FIsBasicDerivedFrom__FP5BASIC3CID(plo, 6)) {
        p->x154 = plo;
        p->x158 = PloAcquireProxy(plo, 0);
        if (!p->x158) {
            p->x154 = 0;
            return 1;
        }
    } else if (FIsBasicDerivedFrom__FP5BASIC3CID(plo, 0x30)) {
        p->x158 = plo;
    } else {
        return 2;
    }
    if (FIsBasicDerivedFrom__FP5BASIC3CID(p->x158, 0x5E)) func_001C7A58(p->x158, id);
    if (FIsBasicDerivedFrom__FP5BASIC3CID(p->x158, 0x5A))
        ((void (*)(void *, int))p->x158->vt[76].pf)((char *)p->x158 + p->x158->vt[76].d, 2);
    else
        ((void (*)(void *))p->x158->vt[9].pf)((char *)p->x158 + p->x158->vt[9].d);
    p->x150 = PxfFindSo(p, pv, 0xD);
    return 3;
}
