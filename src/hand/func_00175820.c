struct U { char pad[0x70]; unsigned long fl70; };
struct O { char pad[0xEC]; struct U *pEC; };
struct Y { int pad; struct O *p4; int pad2[2]; unsigned long fl10; };
struct X { char pad[0x74]; int n74; };
int FIsBasicDerivedFrom__FP5BASIC3CID(struct O *p, int n);
/** @sly2-annot addr=0x175820
 * Purpose: Line-of-sight filter for attack checks: ignore flagged SOs, cid 0x62 objects (when entry asks) and
 *   any STEP
 * Info: proposed name FAttackLosIgnoreSo (T2); TU guardbhv
 * Confidence: low (draft) [T2 0.45] - evidence: static analysis; sources: r9_bhv_screen
 * See: docs/reference/functions/guards-ai.md#guardbhv-0x174898-0x17f090
 * @end-sly2-annot */
int func_00175820(struct X *x, struct Y *p) {
    if (p->fl10 & (1UL << 48)) return 0;
    if (p->p4->pEC->fl70 & 4) return 0;
    if (x->n74 && FIsBasicDerivedFrom__FP5BASIC3CID(p->p4, 0x62)) return 0;
    return !FIsBasicDerivedFrom__FP5BASIC3CID(p->p4, 0x33);
}
