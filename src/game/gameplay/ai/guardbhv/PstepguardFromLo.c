struct O;
struct L { struct O *po; };
struct O { char pad[0x14]; struct L *p14; char pad2[0x1E8]; struct O *p200; };
int FIsBasicDerivedFrom__FP5BASIC3CID(struct O *p, int n);
/** @sly2-annot addr=0x174898
 * Purpose: Return the STEPGUARD that owns an object: the object's LOROOT if it is a STEPGUARD, else (for cid
 *   0x4F children) the root of its +0x200 owner; NULL otherwise
 * Info: proposed name PstepguardFromLo (T1); TU guardbhv
 * Confidence: mid (draft) [T1 0.60] - evidence: static analysis; sources: r9_bhv_screen
 * See: docs/engine/guards-ai.md#8-source-files,
 *   docs/reference/functions/guards-ai.md#guardbhv-0x174898-0x17f090,
 *   docs/reference/tu-map.md#tu-map-address-range---source-file
 * @end-sly2-annot */
struct O *PstepguardFromLo(struct O *p) {
    if (!p) return 0;
    if (FIsBasicDerivedFrom__FP5BASIC3CID(p->p14->po, 0x38)) return p->p14->po;
    if (FIsBasicDerivedFrom__FP5BASIC3CID(p, 0x4F) && FIsBasicDerivedFrom__FP5BASIC3CID(p->p200->p14->po, 0x38)) return p->p200->p14->po;
    return 0;
}
