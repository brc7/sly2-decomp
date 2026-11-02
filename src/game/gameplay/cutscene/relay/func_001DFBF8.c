struct MSG { int cb; int nKind; };
struct S { char pad[0x54]; void *pv; int c; char p1[0x80-0x5C]; int f80; };
void CloneLo(struct S *p);
void *func_001AE238(struct MSG *pmsg, void *pv);
static inline void *PvReallocMsg(struct MSG *pmsg, int cb, int nKind, void *pv) { pmsg->cb = cb; pmsg->nKind = nKind; return func_001AE238(pmsg, pv); }
/** @sly2-annot addr=0x1DFBF8
 * Purpose: RELAY::Clone (vt41): CloneLo; if the relay has a private command limit (+0x80) reallocate its own
 *   copy of the command array
 * Info: proposed name CloneRelay (T1); class RELAY slot41; TU relay
 * Notes: superseded r6_1D5150 proposal: CloneSgg? (0.3)
 * Confidence: mid (draft) [T1 0.70] - evidence: static analysis; sources: r6b_rest
 * See: docs/engine/cutscenes.md#5-key-functions,
 *   docs/reference/functions/cutscenes.md#relay-0x1dfad8-0x1e5d80
 * @end-sly2-annot */
void func_001DFBF8(struct S *p) {
    struct MSG msg;
    CloneLo(p);
    if (p->f80) p->pv = PvReallocMsg(&msg, p->c * 0x50, 0x10, p->pv);
}
