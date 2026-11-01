typedef int qword __attribute__((mode(TI)));
struct S { void *p0; void *p4; int pad[2]; qword q10; };
float SMaxShape(void *p);
void EvaluateShapeS(void *p, qword *pq, int n, float g);
qword func_001A62E8(void *p);
/** @sly2-annot addr=0x1C1F28
 * Purpose: Endpoint position using shape midpoint
 * Info: proposed name PosPathlinkEndpointCenter (T2); TU path
 * Confidence: low (draft) [T2 0.35] - evidence: static analysis; sources: r5b_rest
 * See: docs/reference/functions/physics-collision.md#path-0x1bf110-0x1c2920
 * @end-sly2-annot */
qword func_001C1F28(struct S *p)
{
    qword q;
    if (p->p0) {
        EvaluateShapeS(p->p0, &q, 0, SMaxShape(p->p0) * 0.5f);
    } else if (p->p4) {
        q = func_001A62E8(p->p4);
    } else {
        q = p->q10;
    }
    return q;
}
