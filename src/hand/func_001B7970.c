typedef int qword __attribute__((mode(TI)));
struct VT { short d; short pad; void *pfn; };
struct LO { char pad[0xC]; struct VT *vt; char pad2[0x28 - 0x10]; unsigned long x28; };
struct T { char pad[0x180]; struct LO *po; };
struct P { char pad[0x13F0]; struct T *p13F0; char pad2[0x1414 - 0x13F4]; float g1414; char pad3[0x1420 - 0x1418]; qword v1420; };
void func_001B7D20(struct P *p);
int FIsBasicDerivedFrom__FP5BASIC3CID(struct LO *p, int k);
void func_00209FB8(struct LO *p);
qword func_001A62E8(struct LO *p);
qword GetSoDvGravity(struct LO *p);
qword func_001D69F0(qword a, qword b, qword c, float t);
void func_001F7A68(struct LO *p, int n);
static inline float Dist(qword a, qword b) {
    qword d;
    float f;
    __asm__("vsub.xyzw %0, %1, %2" : "=j"(d) : "j"(a), "j"(b));
    __asm__("vmul.xy %0, %1, %1\n\tvaddy.x %0, %0, %0y" : "=j"(d) : "j"(d));
    __asm__("vsqrt $Q, %1x\n\tvwaitq\n\tvaddq.x %0, $vf0, $Q" : "=j"(d) : "j"(d));
    __asm__("qmfc2.ni %0, %1" : "=r"(f) : "j"(d));
    return f;
}
void func_001B7970(struct P *p)
{
    struct LO *po;
    func_001B7D20(p);
    po = p->p13F0->po;
    p->p13F0 = 0;
    if ((int)(po->x28 >> 60) & 1) {
        float t;
        qword v;
        if (FIsBasicDerivedFrom__FP5BASIC3CID(po, 0x38))
            func_00209FB8(po);
        t = Dist(func_001A62E8(po), p->v1420) / p->g1414;
        v = func_001D69F0(func_001A62E8(po), p->v1420, GetSoDvGravity(po), t);
        ((void (*)(void *, qword))po->vt[35].pfn)((char *)po + po->vt[35].d, v);
        func_001F7A68(po, 2);
    }
}
