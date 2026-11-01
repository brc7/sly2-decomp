struct S { char pad[0x110]; unsigned char ab[26]; };
/** @sly2-annot addr=0x1F78D8
 * Purpose: OR of per-kind response bytes SO+0x110[26] selected by grfzpk
 * Info: proposed name GrfzpdFromSoGrfzpk (T1); TU so
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r6_1D5150+verdicts_tu_r4r6
 * See: docs/engine/physics-collision.md#7-key-functions,
 *   docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8
 * @end-sly2-annot */
int func_001F78D8(struct S *p, int f)
{
    int r = 0; int i; int m;
    for (i = 0, m = 1; i < 26; i++) { if (m & f) r |= p->ab[i]; m <<= 1; }
    return r;
}
