typedef struct MSG { int cb; int nKind; } MSG;
void *func_001AE2A0(MSG *pmsg);
static inline void *PvAllocMsg(MSG *pmsg, int cb, int nKind) { pmsg->cb = cb; pmsg->nKind = nKind; return func_001AE2A0(pmsg); }
struct S { char pad[0x1014]; void *pv; int n; };
void LoadStepFromBrx(struct S *p, void *pv);
void LoadDimitriFromBrx(struct S *p, void *pv)
{
    MSG msg;
    p->pv = PvAllocMsg(&msg, 0x480, 0x10);
    LoadStepFromBrx(p, pv);
    p->n = 0x7FFFFFFF;
}
