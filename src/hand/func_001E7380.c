typedef int qword __attribute__((mode(TI)));
float func_001E64C8(void *p, float seed);
void func_001E6ED8(void *p, qword q, void *a, qword *b, qword *c, float *d, float *e, float g);
/** @sly2-annot addr=0x1E7380
 * Purpose: FindShapeClosestPointFromU(shape, pos, ..., UFromShapeS(shape, s))
 * Info: proposed name FindShapeClosestPointFromS (T1); TU shape
 * Notes: names_v2 C proposed FillCrvcCache__FP4CRVCi (0.438) | top50_round (0.75): Body. Sly 1 crv.h
 *   FindCrvlClosestPointFromS / FindCrvcClosestPointFromS are the same wrapper over ...FromU with the
 *   arc-length->u conversion
 * Confidence: mid (draft) [T1 0.75] - evidence: Sly 1 / Lilly reference; semantic review; static naming pass;
 *   sources: r6b_rest+top50_round
 * See: docs/reference/functions/gameplay-objects.md#tu_1d1370-0x1d1370-0x1d5150,
 *   docs/reference/functions/physics-collision.md#shape-0x1e5d80-0x1ea568
 * @end-sly2-annot */
void func_001E7380(void *p, qword q, void *a, qword *b, qword *c, float *d, float *e, float seed)
{
    func_001E6ED8(p, q, a, b, c, d, e, func_001E64C8(p, seed));
}
