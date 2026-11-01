struct MSG { int cb; int nKind; };
struct S { char pad[0x54]; void *pv; int c; char p1[0x80-0x5C]; int f80; };
void CloneLo(struct S *p);
void *func_001AE238(struct MSG *pmsg, void *pv);
static inline void *PvReallocMsg(struct MSG *pmsg, int cb, int nKind, void *pv) { pmsg->cb = cb; pmsg->nKind = nKind; return func_001AE238(pmsg, pv); }
void func_001DFBF8(struct S *p) {
    struct MSG msg;
    CloneLo(p);
    if (p->f80) p->pv = PvReallocMsg(&msg, p->c * 0x50, 0x10, p->pv);
}
