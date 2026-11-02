typedef int qword __attribute__((mode(TI)));
struct VEC { float x, y, z, w; };
struct VTE { short d; short pad; float *(*pfn)(void *); };
struct O { char pad[0xC]; struct VTE *vt; };
int FIsBasicDerivedFrom__FP5BASIC3CID(struct O *po, int n);
void FindShapeClosestPoint(struct O *po, qword pos, int a, qword *pq, struct VEC *pv, int b, int c);
float atan2f(float y, float x);
qword func_001A62E8(struct O *po);
/** @sly2-annot addr=0x1710F8
 * Purpose: Position and heading of a target: closest point/tangent on a SHAPE (cid 0xA) or the LO position
 *   and yaw
 * Info: proposed name GetCid113TargetPose (T2); class CID113; TU tu_1709A8
 * Notes: Cid71->Cid113 (CID 0x71=113, agent-default(hex))
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r3b_rest
 * See: docs/reference/functions/guards-ai.md#tu_1709a8-0x1709a8-0x171f58
 * @end-sly2-annot */
void func_001710F8(void *pv, qword pos, struct O *po, qword *ppos, float *pg) {
    union { qword q; } u;
#define q u.q
    struct VEC v;
    float f;
    if (po) {
        if (FIsBasicDerivedFrom__FP5BASIC3CID(po, 10)) {
            FindShapeClosestPoint(po, pos, 0, &q, &v, 0, 0);
            f = atan2f(v.y, v.x);
        } else {
            float *a, *b;
            q = func_001A62E8(po);
            a = po->vt[31].pfn((char *)po + po->vt[31].d);
            b = po->vt[31].pfn((char *)po + po->vt[31].d);
            f = atan2f(a[1], b[0]);
        }
    } else {
        q = pos;
        f = 0.0f;
    }
    if (ppos) *ppos = q;
    if (pg) *pg = f;
}
