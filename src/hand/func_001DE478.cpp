struct MSG { int cb; int nKind; };
class FRAME { public: int a[4]; FRAME(); ~FRAME(); };
struct S { char pad[0x198]; int c; void *pv; };
extern "C" void *func_001AE378(MSG *pmsg);
extern "C" void *func_001AE238(MSG *pmsg, void *pv);
extern "C" void LoadSoFromBrx(S *p, void *q);
static inline void *PvAllocMsg(MSG *pmsg, int cb, int nKind) { pmsg->cb = cb; pmsg->nKind = nKind; return func_001AE378(pmsg); }
static inline void *PvReallocMsg(MSG *pmsg, int cb, int nKind, void *pv) { pmsg->cb = cb; pmsg->nKind = nKind; return func_001AE238(pmsg, pv); }
extern "C" void func_001DE478(S *p, void *q)
{
    FRAME frame;
    MSG msg;
    p->pv = PvAllocMsg(&msg, 0x5400, 0x10);
    LoadSoFromBrx(p, q);
    p->pv = PvReallocMsg(&msg, p->c * 0x150, 0x10, p->pv);
}
