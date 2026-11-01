struct LO;
struct VTE { short d; short pad; void (*pf)(); };
struct LO { char pad[0xC]; struct VTE *vt; };
struct G { char pad[0x3EFC]; struct LO *plo3EFC; struct LO *plo3F00; };
struct LO *PpoCur(struct G *p);
#define VCALL(o, n, a) ((void (*)(void *, int))(o)->vt[n].pf)((char *)(o) + (o)->vt[n].d, a)
void func_001C4A60(struct G *p, struct LO *plo)
{
    struct LO *cur = PpoCur(p);
    struct LO *nw = plo;
    if (!nw) nw = p->plo3EFC;
    if (cur != nw && cur) VCALL(cur, 78, 0);
    p->plo3F00 = plo;
    if (cur != nw && nw) VCALL(nw, 78, 1);
}
