typedef int qword __attribute__((mode(TI)));
void FindShapeClosestPoint(void *a, int b, int c, qword *pq, int d, int e, int f);
/** @sly2-annot addr=0x1E77B0
 * Purpose: Return only the closest point of FindShapeClosestPoint
 * Info: proposed name PosFindShapeClosest (T2); TU shape
 * Confidence: low (draft) [T2 0.45] - evidence: static analysis; sources: r6b_rest
 * See: docs/reference/functions/physics-collision.md#shape-0x1e5d80-0x1ea568
 * @end-sly2-annot */
qword func_001E77B0(void *a, int b)
{
    qword q;
    FindShapeClosestPoint(a, b, 0, &q, 0, 0, 0);
    return q;
}
