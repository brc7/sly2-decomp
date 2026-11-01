struct MSG { int cb; int nKind; };
struct S { char pad[0x198]; int c; void *pv; };
void func_001F6068(struct S *p);
void *func_001AE238(struct MSG *pmsg, void *pv);
static inline void *PvReallocMsg(struct MSG *pmsg, int cb, int nKind, void *pv) { pmsg->cb = cb; pmsg->nKind = nKind; return func_001AE238(pmsg, pv); }
/** @sly2-annot addr=0x1DE518
 * Purpose: LASEN::Clone (vt41): Clone__2SO then private copy of the beam array
 * Info: proposed name CloneLasen (T1); class LASEN slot41; TU sensor
 * Notes: superseded r6_1D5150 proposal: CloneLasen? (0.3)
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: r6b_rest
 * See: docs/reference/functions/gameplay-objects.md#sensor-0x1de2a0-0x1dfad8
 * @end-sly2-annot */
void func_001DE518(struct S *p) {
    struct MSG msg;
    func_001F6068(p);
    p->pv = PvReallocMsg(&msg, p->c * 0x150, 0x10, p->pv);
}
