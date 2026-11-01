typedef int qword __attribute__((mode(TI)));
struct VTE { short d; short pad; float *(*pfn)(void *); };
struct O { int pad[3]; struct VTE *vt; char pad2[0x1A8 - 0x10]; float g1A8; char pad3[0x240 - 0x1AC]; qword v240; };
struct P { char pad[0x24]; struct O *po24; char pad2[0x44 - 0x28]; struct O *po44; };
int FIsBasicDerivedFrom__FP5BASIC3CID(void *p, int cid);
qword func_001A62E8(struct O *po);
void FindShapeClosestPoint(struct O *po, qword q, int a, qword *pv, float *pg, int b, int c);
float func_002458F0(float y, float x);
void func_001E4D40(struct P *p, struct O *po, qword *pv, float *pg)
{
    union { qword q; float a[4]; } uv;
    float ag[4];
    float g;
    struct O *px = p->po24;
    if (!px) px = p->po44;
    if (po) {
        if (FIsBasicDerivedFrom__FP5BASIC3CID(po, 0xA)) {
            FindShapeClosestPoint(po, func_001A62E8(px), 0, &uv.q, ag, 0, 0);
            g = func_002458F0(ag[1], ag[0]);
        } else {
            float *p1, *p2;
            uv.q = func_001A62E8(po);
            p1 = po->vt[0x1F].pfn((char *)po + po->vt[0x1F].d);
            p2 = po->vt[0x1F].pfn((char *)po + po->vt[0x1F].d);
            g = func_002458F0(p1[1], p2[0]);
        }
    } else {
        uv.q = func_001A62E8(px);
        if (FIsBasicDerivedFrom__FP5BASIC3CID(px, 0x33)) {
            __asm__("vadd.xyzw %0, %1, %2" : "=j"(uv.q) : "j"(uv.q), "j"(px->v240));
            g = px->g1A8;
        } else {
            float *p1, *p2;
            p1 = px->vt[0x1F].pfn((char *)px + px->vt[0x1F].d);
            p2 = px->vt[0x1F].pfn((char *)px + px->vt[0x1F].d);
            g = func_002458F0(p1[1], p2[0]);
        }
    }
    if (pv) *pv = uv.q;
    if (pg) *pg = g;
}
