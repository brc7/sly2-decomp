// ExplodeExplgExplso (Claude agent, lane D40): matched with Lilly's MATRIX3 operator* (src/game/engine/anim/tail/func_0021CB18.cpp: rows of the LEFT
// matrix into qword locals, VU_VECTOR4 row temps) and her QwRotateVec(MATRIX3 *, qword), with px->mat as the left operand.
typedef int qword __attribute__((mode(TI)));
struct MATRIX4 { qword r[4]; };
struct MATRIX3 { qword r[3]; };
struct VEC { int x, y, z, w; } __attribute__((aligned(16)));
struct XF { qword pos; MATRIX3 mat; VEC v; VEC w; };
struct P { int fl; void *pv; int a8, aC; qword q10, q20; int x30; XF *pxf; int x38, x3C; };
struct VTE { short d; short pad; void (*pfn)(void *, P *); };
struct O { int pad[3]; VTE *vt; char pad1[0x60 - 0x10]; MATRIX4 mat; };
struct S { char pad[0x60]; MATRIX4 mat; char pad1[0xE0 - 0xA0]; int c; O **apo; };
MATRIX4 InvertMatrix4Rigid(MATRIX4 *pm);
MATRIX4 operator*(const MATRIX4 &a, const MATRIX4 &b);
static inline qword Add(qword a, qword b) { qword d; __asm__("vadd.xyzw %0, %1, %2" : "=j"(d) : "j"(a), "j"(b)); return d; }
struct VU_VECTOR4 { qword qw; };
inline MATRIX3 operator*(const MATRIX3 &matL, const MATRIX3 &matR) return mat
{
    qword qwL0 = matL.r[0];
    qword qwL1 = matL.r[1];
    qword qwL2 = matL.r[2];
    VU_VECTOR4 v0;
    VU_VECTOR4 v1;
    VU_VECTOR4 v2;
    __asm__("vmulax.xyz $ACC, %1, %4x\n\tvmadday.xyz $ACC, %2, %4y\n\tvmaddz.xyz %0, %3, %4z"
            : "=j"(v0.qw) : "j"(qwL0), "j"(qwL1), "j"(qwL2), "j"(matR.r[0]));
    *(VU_VECTOR4 *)&mat.r[0] = v0;
    __asm__("vmulax.xyz $ACC, %1, %4x\n\tvmadday.xyz $ACC, %2, %4y\n\tvmaddz.xyz %0, %3, %4z"
            : "=j"(v1.qw) : "j"(qwL0), "j"(qwL1), "j"(qwL2), "j"(matR.r[1]));
    *(VU_VECTOR4 *)&mat.r[1] = v1;
    __asm__("vmulax.xyz $ACC, %1, %4x\n\tvmadday.xyz $ACC, %2, %4y\n\tvmaddz.xyz %0, %3, %4z"
            : "=j"(v2.qw) : "j"(qwL0), "j"(qwL1), "j"(qwL2), "j"(matR.r[2]));
    *(VU_VECTOR4 *)&mat.r[2] = v2;
}
inline qword QwRotateVec(const MATRIX3 *pmat, qword qwVec)
{
    qword qwResult;
    __asm__("vmulax.xyz $ACC, %1, %4x\n\tvmadday.xyz $ACC, %2, %4y\n\tvmaddz.xyz %0, %3, %4z"
            : "=j"(qwResult)
            : "j"(pmat->r[0]), "j"(pmat->r[1]), "j"(pmat->r[2]), "j"(qwVec));
    return qwResult;
}
extern "C" void ExplodeExplgExplso(S *p, P *pp);
/** @sly2-annot addr=0x15C3C0
 * Purpose: EXPLG explode: transforms the explosion origin by its matrix and forwards the explode call to
 *   every child explosion (vtable)
 * Info: proposed name ExplodeExplgExplso (T1); class EXPLG slot51; TU emitter
 * Confidence: mid (draft) [T1 0.60] - evidence: static analysis; sources: r3_1539A8
 * See: docs/reference/functions/particles-effects.md#emitter-0x159118-0x15cd68
 * @end-sly2-annot */
void ExplodeExplgExplso(S *p, P *pp)
{
    P loc = *pp;
    XF xf;
    loc.pxf = &xf;
    loc.fl |= 0x40;
    if (loc.fl & 1) {
    } else {
        loc.pv = p;
        loc.fl |= 1;
    }
    for (int i = 0; i < p->c; i++) {
        O *po = p->apo[i];
        MATRIX4 mRel;
        mRel = InvertMatrix4Rigid(&p->mat) * po->mat;
        MATRIX3 &m3 = *(MATRIX3 *)&mRel;
        if (pp->fl & 0x40) {
            XF *px = pp->pxf;
            xf.mat = px->mat * m3;
            xf.pos = Add(px->pos, QwRotateVec(&px->mat, mRel.r[3]));
            xf.v = px->v;
            xf.w = px->w;
        } else {
            xf.mat = m3;
            xf.pos = mRel.r[3];
            xf.v.x = xf.v.y = xf.v.z = 0;
            xf.w.x = xf.w.y = xf.w.z = 0;
        }
        po->vt[51].pfn((char *)po + po->vt[51].d, &loc);
    }
}
