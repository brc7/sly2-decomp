typedef int qword __attribute__((mode(TI)));
struct VEC { float x, y, z, w; };
struct VTE { short d; short pad; float *(*pfn)(void *); };
struct O { char pad[0xC]; struct VTE *vt; };
int FIsBasicDerivedFrom__FP5BASIC3CID(struct O *po, int n);
void FindShapeClosestPoint(struct O *po, qword pos, int a, qword *pq, struct VEC *pv, int b, int c);
float func_002458F0(float y, float x);
qword func_001A62E8(struct O *po);
void func_001710F8(void *pv, qword pos, struct O *po, qword *ppos, float *pg) {
    union { qword q; } u;
#define q u.q
    struct VEC v;
    float f;
    if (po) {
        if (FIsBasicDerivedFrom__FP5BASIC3CID(po, 10)) {
            FindShapeClosestPoint(po, pos, 0, &q, &v, 0, 0);
            f = func_002458F0(v.y, v.x);
        } else {
            float *a, *b;
            q = func_001A62E8(po);
            a = po->vt[31].pfn((char *)po + po->vt[31].d);
            b = po->vt[31].pfn((char *)po + po->vt[31].d);
            f = func_002458F0(a[1], b[0]);
        }
    } else {
        q = pos;
        f = 0.0f;
    }
    if (ppos) *ppos = q;
    if (pg) *pg = f;
}
