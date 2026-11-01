typedef int qword __attribute__((mode(TI)));
typedef union { qword q; float f[4]; } VU;
typedef struct { qword r[3]; } M3;
struct O { char pad[0x190]; qword v190; char pad1[0x1B0 - 0x1A0]; M3 m1B0; char pad2[0x10DC - 0x1E0]; void *p10DC; };
struct CLOCK { float t; float dt; };
extern qword func_001A62E8(void *p);
extern void func_002046B0(struct O *p, qword v);
extern float func_0011D018(float g, float gTarget, float dt, void *pv, float *pg);
extern M3 *func_0011B340(M3 *pmOut, M3 *pm);
extern struct CLOCK D_002DDED0;
extern float D_002BF414;
extern char D_002BF418[];
extern qword D_002A2DA0;
static __inline__ qword VSub(qword a, qword b) { qword r; __asm__("vsub.xyzw %0, %1, %2" : "=j"(r) : "j"(a), "j"(b)); return r; }
static __inline__ qword VLen(qword c)
{
    qword l, one;
    __asm__("vmul.xyz %1, %2, %2\n\tvaddw.x %0, $vf0, $vf0w\n\tvadday.x ACC, %1, %1y\n\tvmaddz.x %1, %0, %1z" : "=&j"(one), "=&j"(l) : "j"(c));
    __asm__("vsqrt Q, %1x\n\tvwaitq\n\tvaddq.x %0, $vf0, Q" : "=j"(l) : "j"(l));
    return l;
}
/** @sly2-annot addr=0x18E1E0
 * Purpose: Turn Sly toward object x10DC with GSmooth
 * Info: proposed name UpdateJtFaceTarget (T2); TU jt
 * Notes: names_v2 C proposed UpdateRop__FP3ROPf (0.438)
 * Confidence: low (draft) [T2 0.30] - evidence: static naming pass; sources: r4_17F090
 * See: docs/reference/functions/player.md#jt-0x188488-0x18e540
 * @end-sly2-annot */
void func_0018E1E0(struct O *p)
{
    VU n;
    M3 ms, m;
    VU u;
    float g;
    qword d, l, s, r;
    float gl, gLen;
    qword *pk;
    d = VSub(func_001A62E8(p->p10DC), func_001A62E8(p));
    pk = &D_002A2DA0;
    l = VLen(d);
    n.q = d;
    __asm__("qmfc2.ni %0, %1" : "=r"(gl) : "j"(l));
    if (gl < 1e-4f) {
        n.q = *pk;
    } else {
        __asm__("vdiv Q, $vf0w, %2x\n\tvwaitq\n\tvmulq.xyzw %0, %1, Q" : "=j"(r) : "j"(d), "j"(l));
        n.q = r;
    }
    __asm__("qmfc2.ni %0, %1" : "=r"(gLen) : "j"(l));
    func_002046B0(p, n.q);
    func_0011D018(gLen, D_002BF414, D_002DDED0.dt * 0.5f, D_002BF418, &g);
    func_0011B340(&m, &p->m1B0);
    __asm__("qmtc2.ni %1, %0" : "=j"(s) : "r"(*(int *)&g));
    u.q = s;
    __asm__("vmulx.xyzw %0, %1, %2x" : "=j"(ms.r[0]) : "j"(m.r[0]), "j"(s));
    __asm__("vmulx.xyzw %0, %1, %2x" : "=j"(ms.r[1]) : "j"(m.r[1]), "j"(s));
    __asm__("vmulx.xyzw %0, %1, %2x" : "=j"(ms.r[2]) : "j"(m.r[2]), "j"(s));
    __asm__("vmulax.xyz ACC, %1, %4x\n\tvmadday.xyz ACC, %2, %4y\n\tvmaddz.xyz %0, %3, %4z"
        : "=j"(r) : "j"(ms.r[0]), "j"(ms.r[1]), "j"(ms.r[2]), "j"(n.q));
    p->v190 = r;
}
