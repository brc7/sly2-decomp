typedef struct MSG { int cb; int nKind; } MSG;
void *func_001AE238(MSG *pmsg, void *pv);
static inline void *PvReallocMsg(MSG *pmsg, void *pv, int cb, int nKind) { pmsg->cb = cb; pmsg->nKind = nKind; return func_001AE238(pmsg, pv); }
struct S { int pad[4]; void *p10; char pad2[0xE0 - 0x14]; int c; void **apv; };
void CloneXfm(struct S *p, void *pv);
void *PloCloneLo__FP2LOP2SO(void *pv, void *p10);
void func_0015C320(struct S *p, void *pv)
{
    MSG msg;
    int i;
    CloneXfm(p, pv);
    p->apv = PvReallocMsg(&msg, p->apv, p->c * 4, 0x10);
    for (i = 0; i < p->c; i++)
        p->apv[i] = PloCloneLo__FP2LOP2SO(p->apv[i], p->p10);
}
