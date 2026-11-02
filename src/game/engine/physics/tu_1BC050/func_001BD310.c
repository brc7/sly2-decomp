typedef int qword __attribute__((mode(TI)));
struct R { char pad[0x40]; float f40; float f44; char pad2[0x78]; };
int func_001C1700(void *a, void *b, qword q2, qword q3, int c, void **buf);
int ClsgClipEdgeToCnvo(void *e, qword q2, qword q3, int n, struct R *r);
/** @sly2-annot addr=0x1BD310
 * Purpose: Among obstacles overlapping a segment, return the first one the segment enters
 * Info: proposed name PcnvoFirstHitSegment (T2); TU tu_1BC050
 * Notes: names_v2 B proposed UpdateJtTool__FP2JT (0.856)
 * Confidence: low (draft) [T2 0.40] - evidence: static naming pass; sources: r5b_rest
 * See: docs/reference/functions/physics-collision.md#tu_1bc050-0x1bc050-0x1be398
 * @end-sly2-annot */
void *func_001BD310(void *a, void *b, qword q2, qword q3)
{
    void *buf[0x80];
    struct R r;
    void *best = 0;
    float min;
    int i, n;
    n = func_001C1700(a, b, q2, q3, 0x80, buf);
    min = 1.0f;
    for (i = 0; i < n; i++) {
        void *e = buf[i];
        if (!ClsgClipEdgeToCnvo(e, q2, q3, 2, &r) || r.f40 != 0.0f) {
            best = e;
            break;
        }
        if (r.f44 >= min) continue;
        min = r.f44;
        best = e;
    }
    return best;
}
