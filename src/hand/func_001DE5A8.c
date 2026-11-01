struct S { char pad[0x70]; unsigned long fl; };
struct S *func_001F56F8(void *p);
/** @sly2-annot addr=0x1DE5A8
 * Purpose: LASEN vt53: SO::PtreeNew then set tree flag 0x20
 * Info: proposed name LASEN_PtreeNew (T2); class LASEN slot53; TU sensor
 * Notes: names_v2 C proposed SetLightFrustrumUp__FP5LIGHTP6VECTOR (0.167)
 * Confidence: low (draft) [T2 0.40] - evidence: static naming pass; sources: r6b_rest
 * See: docs/reference/functions/gameplay-objects.md#sensor-0x1de2a0-0x1dfad8
 * @end-sly2-annot */
struct S *func_001DE5A8(void *p) {
    struct S *ps = func_001F56F8(p);
    ps->fl |= 0x20;
    return ps;
}
