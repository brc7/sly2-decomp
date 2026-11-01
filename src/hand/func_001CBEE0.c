struct R { float lo, hi; };
struct T { char pad[0x50]; int x50; float x54; struct R r; };
struct U { char pad[0x18]; int x18; };
struct S { char pad[0x14]; struct T *pt; char p1[0x40 - 0x18]; struct U *pu; };
int func_00208BE0(void *p, struct S *ps);
/** @sly2-annot addr=0x1CBEE0
 * Purpose: CID 61 vt89: for stimulus 0x246 temporarily double the sense range/arc, then STEPGUARD
 *   FStepguardSenseStimulus 0x208BE0
 * Info: proposed name Rajan_FSenseStimulus (T2); class RAJAN slot89; TU tu_1CBEB0
 * Notes: class identity from whole_game WE names/object strings: Cid61Guard_FSenseStimulus ->
 *   Rajan_FSenseStimulus
 * Confidence: low (draft) [T2 0.45] - evidence: static analysis; sources: r5b_rest+whole_game(class identity)
 * See: docs/reference/functions/guards-ai.md#tu_1cbeb0-0x1cbeb0-0x1cc158
 * @end-sly2-annot */
int func_001CBEE0(void *p, struct S *ps)
{
    struct T *pt = ps->pt;
    struct R r = pt->r;
    float f = pt->x54;
    int ret;
    if (pt->x50 == 0 && ps->pu && ps->pu->x18 == 0x246) {
        float d, c;
        d = (r.hi - r.lo) * 0.5f;
        c = r.lo + d;
        pt->x54 = f + f;
        d = d + d;
        pt->r.hi = c + d;
        pt->r.lo = c - d;
    }
    ret = func_00208BE0(p, ps);
    pt->r = r;
    pt->x54 = f;
    return ret;
}
