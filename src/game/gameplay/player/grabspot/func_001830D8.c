typedef int qword __attribute__((mode(TI)));
typedef union { qword q; float f[4]; } VU;
struct J { char pad[0x240]; qword v240; char pad2[0x103C - 0x250]; float g103C; int n1040; };
extern void EvaluateShapeS(void *p, float t, VU *ppos, VU *pdir);
extern float atan2f(float y, float x);
extern qword D_002A2DC0;
extern qword D_002A2DA0;
static __inline__ float GetX(qword q) { float x; __asm__("mtc1 %1, %0" : "=f"(x) : "r"(q)); return x; }
static __inline__ float GetY(qword q) { float y; __asm__("prot3w %0, %1" : "=r"(y) : "r"(q)); return y; }
static __inline__ qword VLen(qword c)
{
    qword l, one;
    __asm__("vmul.xyz %0, %2, %2\n\tvaddw.x %1, $vf0, $vf0w\n\tvadday.x ACC, %0, %0y\n\tvmaddz.x %0, %1, %0z" : "=&j"(l), "=&j"(one) : "j"(c));
    __asm__("vsqrt Q, %1x\n\tvwaitq\n\tvaddq.x %0, $vf0, Q" : "=j"(l) : "j"(l));
    return l;
}
static __inline__ qword Bc(int g)
{
    VU u;
    qword s;
    __asm__ __volatile__("qmtc2.ni %1, %0" : "=j"(s) : "r"(g));
    u.q = s;
    return s;
}
static __inline__ qword VNormalize(qword c, qword k)
{
    qword l;
    register qword t __asm__("vf1");
    float gl;
    l = VLen(c);
    __asm__("qmfc2.ni %0, %1" : "=r"(gl) : "j"(l));
    if (gl < 1e-4f) {
        __asm__("" : "+r"(k));
        t = k;
    } else {
        __asm__ __volatile__("nop\n\tnop\n\tvdiv Q, $vf0w, %2x\n\tvwaitq\n\tvmulq.xyzw %0, %1, Q" : "=j"(t) : "j"(c), "j"(l));
    }
    return t;
}
/** @sly2-annot addr=0x1830D8
 * Purpose: Position and facing (atan2 of tangent x up) on the shape at param g
 * Info: proposed name GetCid12GrabPos (T2); class CID12; TU grabspot
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r4b_rest
 * See: docs/reference/functions/player.md#grabspot-0x182c70-0x1848a8
 * @end-sly2-annot */
void func_001830D8(void *p, float g, struct J *pj, qword *pv, float *pg)
{
    VU pos, dir;
    qword n;
    qword c, r;
    float gl;
    EvaluateShapeS(p, g, &pos, &dir);
    __asm__("vopmula.xyz ACC, %1, %2\n\tvopmsub.xyz %0, %2, %1" : "=j"(c) : "j"(dir.q), "j"(D_002A2DC0));
    n = VNormalize(c, D_002A2DA0);
    if (pv) {
        qword s;
        VU u;
        register qword a __asm__("vf2");
        register qword b __asm__("vf3");
        __asm__ __volatile__("qmtc2.ni %1, %0" : "=j"(s) : "r"(*(int *)&pj->g103C));
        a = pos.q;
        __asm__ __volatile__("");
        b = pj->v240;
        __asm__("vadda.xyzw ACC, %1, $vf0\n\tvmaddx.xyzw %0, %2, %3x\n\tvsub.xyzw %0, %0, %4" : "=j"(a) : "j"(a), "j"(n), "j"(s), "j"(b));
        u.q = s;
        *pv = a;
    }
    if (pg) {
        if (pj->n1040 == 1) {
            float m = -1.0f;
            qword s = Bc(*(int *)&m);
            __asm__("vmulx.xyzw %0, %1, %2x" : "=j"(n) : "j"(n), "j"(s));
        }
        *pg = atan2f(GetY(n), GetX(n));
    }
}
