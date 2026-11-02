typedef int qword __attribute__((mode(TI)));
static __inline__ float GetX(qword q) { float x; __asm__("mtc1 %1,%0" : "=f"(x) : "r"(q)); return x; }
static __inline__ float GetY(qword q) { float y; __asm__("prot3w %0,%1" : "=r"(y) : "r"(q)); return y; }
static __inline__ float GetZ(qword q) { float z; __asm__("pextuw %0,$0,%1" : "=r"(z) : "r"(q)); return z; }
struct O { char pad[0xE0]; struct O *pnext; int x4; int xE8; };
struct P { char pad[0x1A8]; float g1A8; char pad1[0x13F0 - 0x1AC]; void *p13F0; char pad2[0x1410 - 0x13F4]; float g1410;
           char pad3[0x1420 - 0x1414]; qword v1420; };
struct G { char pad[0x13B4]; struct O *pFirst; };
extern struct G D_003E0000;
extern float D_002C0984, D_002C0988;
qword func_001A62E8(void *p);
int func_00184380(void *a, struct O *po);
float atan2f(float y, float x);
float RadStepHeading(struct P *p);
float RadNormalize__Ff(float g);
qword SetVectorCylind(float a, float b, float c);
static __inline__ float GetZ2(qword t) { __asm__("pextuw %0,$0,%0" : "+r"(t)); return GetX(t); }
static __inline__ float GetY2(qword t) { __asm__("prot3w %0,%0" : "+r"(t)); return GetX(t); }
static inline qword ToGpr(qword v) { qword r; __asm__ __volatile__("qmfc2.ni %0, %1" : "=r"(r) : "j"(v)); return r; }
static inline float LenXY(qword v) {
    qword d;
    float f;
    __asm__("vmul.xy %0, %1, %1\n\tvaddy.x %0, %0, %0y" : "=j"(d) : "j"(v));
    __asm__("vsqrt $Q, %1x\n\tvwaitq\n\tvaddq.x %0, $vf0, $Q" : "=j"(d) : "j"(d));
    __asm__("qmfc2.ni %0, %1" : "=r"(f) : "j"(d));
    return f;
}
/** @sly2-annot addr=0x1B5E98
 * Purpose: Pick throw goal v1420: nearest object with flag 0x10000 (not the carried one) within range
 *   g1410*k, >150 away, <500 above, inside a facing cone; else a point g1410 ahead
 * Info: proposed name ChooseMurrayThrowTarget (T1); TU murray
 * Notes: runtime5a #20/#21: fallback goal 1500 units ahead (S43)
 * Confidence: mid (draft) [T1 0.60] - evidence: runtime (save-state RAM); sources: r5b_rest+runtime
 * See: docs/reference/functions/player.md#murray-0x1b4728-0x1b9f80
 * @end-sly2-annot */
void ChooseMurrayThrowTarget(struct P *p)
{
    struct { int a[4]; } unused;
    struct O *po, *poBest = 0;
    float gMax = p->g1410 * D_002C0984;
    for (po = D_003E0000.pFirst; po; po = po->pnext) {
        qword d;
        float dist, g;
        if (!(po->xE8 & 0x10000))
            continue;
        if (func_00184380(p->p13F0, po))
            continue;
        __asm__("vsub.xyzw %0, %1, %2" : "=j"(d) : "j"(func_001A62E8(po)), "j"(func_001A62E8(p)));
        dist = LenXY(d);
        if (gMax < dist)
            continue;
        if (dist < 150.0f)
            continue;
        if (500.0f < GetZ2(ToGpr(d)))
            continue;
        { qword t = ToGpr(d); float x = GetX(t); __asm__ __volatile__("prot3w %0,%0" : "+r"(t)); g = atan2f(GetX(t), x); }
        if (D_002C0988 < __builtin_fabsf(RadNormalize__Ff(g - RadStepHeading(p))))
            continue;
        poBest = po;
        gMax = dist;
        p->v1420 = func_001A62E8(poBest);
    }
    if (!poBest) {
        qword a, b, r;
        a = func_001A62E8(p);
        b = SetVectorCylind(p->g1A8, p->g1410, 0.0f);
        __asm__("vadd.xyzw %0, %1, %2" : "=j"(r) : "j"(a), "j"(b));
        p->v1420 = r;
    }
}
