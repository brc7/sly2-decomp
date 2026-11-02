struct S { char pad[0x110]; unsigned char ab[26]; };
/** @sly2-annot addr=0x1F7918
 * Purpose: Set SO+0x110[i]=b for each kind bit in grfzpk
 * Info: proposed name SetSoZapResponse (T2); TU so
 * Notes: names_v2 v1 proposed FIgnoreSoIntersection__FP2SOT0 (0.017)
 * Confidence: low (draft) [T2 0.40] - evidence: static naming pass; sources: r6_1D5150
 * See: docs/engine/game-progression.md#8-key-functions,
 *   docs/reference/functions/game-progression.md#prize-0x1c4e50-0x1c8cc0,
 *   docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8
 * @end-sly2-annot */
void func_001F7918(struct S *p, int f, int b)
{
    int i; int m;
    for (i = 0, m = 1; i < 26; i++) { if (m & f) p->ab[i] = b; m <<= 1; }
}
