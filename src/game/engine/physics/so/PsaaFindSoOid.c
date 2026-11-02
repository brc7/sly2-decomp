struct T { char pad[0x18]; int n; };
struct S { char pad[0xA4]; int c; struct T **apt; };
/** @sly2-annot addr=0x1F8198
 * Purpose: Find SAA in +0xA4 list with +0x18 == n
 * Info: proposed name PsaaFindSoOid (T1); TU so
 * Notes: verifier CORRECT: PsaaFindSo -> PsaaFindSoOid
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r6_1D5150+verdicts_tu_r4r6
 * See: docs/engine/physics-collision.md#7-key-functions,
 *   docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8
 * @end-sly2-annot */
struct T *PsaaFindSoOid(struct S *p, int n)
{
    int i;
    for (i = 0; i < p->c; i++) { struct T *pt = p->apt[i]; if (pt->n == n) return pt; }
    return 0;
}
