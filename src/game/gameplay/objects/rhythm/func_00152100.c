struct VTE { short d; short pad; void (*pfn)(); };
struct O { int a[3]; struct VTE *vt; };
void PostStepLoad(struct O *o);
void func_00152578(struct O *o);
/** @sly2-annot addr=0x152100
 * Purpose: cid 0x44 vt15: base 0x203A38, BindDancerMoves (resolve aseg oids), vt79(0)
 * Info: proposed name PostLoadDancer (T2); class DANCER slot15; TU rhythm
 * Notes: verifier: purpose partly wrong: CID 0x44 vtable 0x2CD748 slot 15 (ref 0x2CD7C4); base 0x203A38,
 *   0x152578, vt79(0). 0x152578 resolves up to 8x3 oids via 0x152520 = PloFindSwObject(4) then calls
 * Confidence: low (draft) [T2 0.40] - evidence: semantic review; sources: r2_131F70+verdicts_tu_r1r2
 * See: docs/engine/gameplay-objects.md#key-functions,
 *   docs/reference/functions/gameplay-objects.md#rhythm-0x1520c8-0x1539a8
 * @end-sly2-annot */
void func_00152100(struct O *o)
{
    struct VTE *e;
    PostStepLoad(o);
    func_00152578(o);
    e = &o->vt[79];
    e->pfn((char *)o + e->d, 0);
}
