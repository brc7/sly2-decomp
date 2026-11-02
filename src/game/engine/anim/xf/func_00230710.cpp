// (Claude agent, lane E17) MATCH from fresh19: func_0022CC00 declared void here. With the M4* return, the call sets $v0, so the
// r3 store (v0) lost sched2's "more dependents" tie-break to the r2 store (v1, reused after the call). With void, plain *pm = t
// matches and the old r1/$7 pin and qword temps are gone.
// (Claude agent, lane E7) fresh18: aligned 2 (fresh17 3). SetM4 copies mat through explicit qword temps r0,r1,r2 stored r1,r2,r0.
// Left: path-3 *pm = t (target loads r0,r2,r3,r1, stores r3,r2,r0,r1); all 24 temp store orders x load orders tried, Q4 struct copies: 2.
// (Claude agent, lane E6) fresh17: aligned 3 (fresh16 6). (1) VUF ctor: `float f = g; asm("" : "+f"(f)); qmtc2 "r"(f)`:
// gcc emits the mfc1 itself (with its latency, so a free insn lands between mfc1 and qmtc2 like the target) and g stays in f20.
// (2) path 2 uses VUF2, the same ctor with a VOLATILE empty asm (keeps the qB0 lqc2 after the mfc1), through
// BuildRotationS(const VUF2 &f, qword v) { func_0011B570(&m, Scale(v, f)); }. Left: SetM4 row store order (r1,r2,r0) and path-3 *pm = t store order.
typedef int qword __attribute__((mode(TI)));
typedef union { qword r[4]; float m[4][4]; } M4;
struct M3 { qword r[3]; };
struct VT { short d; short pad; void (*pfn)(); };
struct OB { char pad[0x18]; VT *vt; };
struct O {
    M4 local; char pad40[0x90 - 0x40]; OB *pob; int k94; short k98; short k9A; char pad9C[0xB0 - 0x9C]; qword qB0;
};
extern "C" {
int func_00230310(int);
M4 *func_0022C8C0(M4 *pm, O *po, qword v);
M4 *func_0022CB90(M4 *pr, O *pa, const M4 *basis, qword c, qword d);
void func_0022CC00(M4 *pm, O *po, qword v);
M4 *func_0022CCD0(M4 *pm, O *pa, const M4 *basis, qword v);
M3 *func_0011B570(M3 *, qword);
qword func_0022CA18(O *pa, const M4 *matrix);
qword *VECTOR_opAssign(qword *, qword);
void *func_002333B4(void *, int, int);
void func_00230710(O *po, M4 *pm, M4 *pm2, float g);
}
struct VUF { qword q; VUF(float g) { float f = g; __asm__("" : "+f"(f)); __asm__("qmtc2.ni %1, %0" : "=j"(q) : "r"(f)); } };
struct VUF2 { qword q; VUF2(float g) { float f = g; __asm__ __volatile__("" : "+f"(f)); __asm__("qmtc2.ni %1, %0" : "=j"(q) : "r"(f)); } operator const VUF &() const { return *(const VUF *)this; } };
static inline M3 BuildRotation(qword v) return m { func_0011B570(&m, v); }
static inline M3 MulM3(const M3 &a, const M3 &b) return m
{
    M3 t;
    qword r0, r1, r2;
    __asm__("vmulax.xyz $ACC, %1, %4x\n\tvmadday.xyz $ACC, %2, %4y\n\tvmaddz.xyz %0, %3, %4z"
            : "=j"(r0) : "j"(a.r[0]), "j"(a.r[1]), "j"(a.r[2]), "j"(b.r[0]));
    t.r[0] = r0; m.r[0] = r0;
    __asm__("vmulax.xyz $ACC, %1, %4x\n\tvmadday.xyz $ACC, %2, %4y\n\tvmaddz.xyz %0, %3, %4z"
            : "=j"(r1) : "j"(a.r[0]), "j"(a.r[1]), "j"(a.r[2]), "j"(b.r[1]));
    t.r[1] = r1; m.r[1] = r1;
    __asm__("vmulax.xyz $ACC, %1, %4x\n\tvmadday.xyz $ACC, %2, %4y\n\tvmaddz.xyz %0, %3, %4z"
            : "=j"(r2) : "j"(a.r[0]), "j"(a.r[1]), "j"(a.r[2]), "j"(b.r[2]));
    t.r[2] = r2; m.r[2] = r2;
}
static inline void SetM4(M4 *pm, const M3 &mat, qword pos)
{
    VECTOR_opAssign(&pm->r[3], pos);
    qword r0 = mat.r[0], r1 = mat.r[1], r2 = mat.r[2];
    pm->r[1] = r1; pm->r[2] = r2; pm->r[0] = r0;
    pm->m[3][3] = 1.0f;
    pm->m[2][3] = 0.0f;
    pm->m[1][3] = 0.0f;
    pm->m[0][3] = 0.0f;
}
static inline M4 C8c0(O *po, const qword &v) return r { func_0022C8C0(&r, po, v); }
static inline M4 Cc00(O *po, const qword &v) return r { func_0022CC00(&r, po, v); }
static inline M4 Ccd0(O *po, const M4 *pm, qword v) return r { func_0022CCD0(&r, po, pm, v); }
static inline qword Scale(qword v, const VUF &f)
{
    qword r;
    __asm__("vmulx.xyzw %0, %2, %1x" : "=&j"(r) : "j"(f.q), "j"(v));
    return r;
}
static inline M3 BuildRotationS(const VUF2 &f, qword v) return m { func_0011B570(&m, Scale(v, *(const VUF *)&f)); }
static inline qword AddScale(qword a, qword v, const VUF &f)
{
    qword r;
    __asm__("vadda.xyzw $ACC, %1, $vf0\n\tvmaddx.xyzw %0, %2, %3x" : "=&j"(r) : "j"(a), "j"(v), "j"(f.q));
    return r;
}
/** @sly2-annot addr=0x230710
 * Purpose: UNKNOWN - no analysis recorded yet (byte-matched only).
 * Info: TU xf
 * Notes: descriptive only: xf: apply job override to matrix | no usable identifier; purpose only
 * Confidence: name low [T2 0.20]; purpose not reviewed - evidence: static analysis
 * @end-sly2-annot */
void func_00230710(O *po, M4 *pm, M4 *pm2, float g)
{
    if (po->pob) {
        qword q[2];
        M4 t;
        OB *pob = po->pob;
        ((void (*)(void *, qword *, qword *))pob->vt[5].pfn)((char *)pob + pob->vt[5].d, &q[0], &q[1]);
        func_0022C8C0(&t, po, q[0]);
        *pm = t;
        switch (po->k94) {
        case 6: case 7:
            func_0022CB90(&t, po, pm, q[0], q[1]);
            *pm2 = t;
            break;
        default:
            func_0022CC00(&t, po, q[1]);
            *pm2 = t;
            break;
        }
    } else if (func_00230310(po->k98)) {
        switch (po->k94) {
        case 6: case 7: {
            M3 mat;
            mat = MulM3(BuildRotationS(VUF2(g), po->qB0), *(M3 *)&po->local);
            SetM4(pm, mat, po->local.r[3]);
            *pm2 = Ccd0(po, pm, po->qB0);
            break;
        }
        default: {
            qword v[1];
            v[0] = AddScale(func_0022CA18(po, &po->local), po->qB0, VUF(g));
            M4 t;
            func_0022C8C0(&t, po, v[0]);
            *pm = t;
            func_0022CC00(&t, po, po->qB0);
            *pm2 = t;
            break;
        }
        }
    } else {
        *pm = po->local;
        func_002333B4(pm2, 0, 0x40);
    }
}
