typedef int qword __attribute__((mode(TI)));
int ClsgClipEdgeToObjectPruned(void *p, int n, qword a, qword b, int c, float *d);
/** @sly2-annot addr=0x14C6A0
 * Purpose: Clip segment against an SO: ClsgClipEdgeToObjectPruned(pso, pso->bsp (+0x74), ...)
 * Info: proposed name ClsgClipEdgeToObject (T1); TU clip
 * Confidence: high [T1 0.80] - evidence: static analysis; sources: r2b_rest
 * See: docs/engine/gameplay-objects.md#source-files, docs/engine/physics-collision.md#4-geometry-and-queries,
 *   docs/engine/physics-collision.md#6-source-files
 * @end-sly2-annot */
int ClsgClipEdgeToObject(char *p, qword a, qword b, int c, float *d)
{
    return ClsgClipEdgeToObjectPruned(p, *(int *)(p + 0x74), a, b, c, d);
}
