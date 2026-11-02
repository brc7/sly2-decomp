// ProjectBlipgTransformAccel (0x139AB8): DMA-double-buffered VU particle update over the DLI list at p->dl (0x448), accumulating the
// bounding box of every particle batch in vMin/vMax (CblipeProjectBlipTransformAccel, microprogram), then sets the bounding sphere
// p->vF0 = centre, p->g100 = radius + p->g120 (or calls virtual slot 9 when p->c444 drops to 0).
// MATCH (Claude agent for Lilly, T1675, 2026-10-25), built on lilly2 (T1663: arg block) and lilly1 (T1659: sceDmaSync returns int).
// The prologue (hi $LC0 v0, zero v1, hi $LC1 v0, f0 stores then f1 stores) needs VECTOR = struct with an anonymous union:
// store_constructor clears the struct (sq zero) and then ALWAYS emits a CLOBBER for the union member, even when already
// cleared (expr.c, UNION_TYPE). That clobber is deleted by reload, so it only exists for sched1: it sits between lwc1 A
// and lui B in the sched1 stream, which lets local-alloc give both pool his $v0 (and the zero reload $v1).
typedef int qword __attribute__((mode(TI)));
#define FLT_MAX 3.40282347e+38f /* $LC0/$LC1 at 0x2A4AF4/0x2A4AF8: this function's literal pool */
struct VECTOR { union { struct { float x, y, z; } v; qword q; }; };
struct M4 { qword a[4]; };
struct DL { void *head; void *tail; int ibDle; };
struct DLI { DL *m_pdl; void **m_ppv; int m_ibDle; DLI *m_pdliNext; };
struct BUF { char ab[0x2B0]; char ab2[0x560 - 0x2B0]; char ab3[0x840 - 0x560]; };
struct Q { BUF abuf[2]; int i; int c; };
struct SBE { int cb; char *pv; };
struct SPRBUF { unsigned long *aqw; char *pv; SBE a[8]; int c; int state; };
struct VU_FLOAT { qword qw; VU_FLOAT(float f) { __asm__("qmtc2.ni %1, %0" : "=j"(qw) : "r"(f)); } VU_FLOAT() {} };
struct VU_FLOATM { qword qw; };

struct P {
    int pad0[3];
    virtual void f1(); virtual void f2(); virtual void f3(); virtual void f4(); virtual void f5();
    virtual void f6(); virtual void f7(); virtual void f8(); virtual void f9();
};
struct PX : P {
    char pad[0xF0 - 0x10]; qword vF0; float g100; char pad104[0x120 - 0x104]; float g120;
    char pad124[0x170 - 0x124]; char x170[0x43C - 0x170]; int x43C; int pad440; int c444; DL dl;
};
extern "C" {
extern DLI *D_002DD2FC;
extern float D_002DDED0[];
extern int D_002C3C5C;
extern SPRBUF D_003DD2C0[2];
extern void *D_002DE59C;
}
void LoadSprBuf(int cb, ...);
void SaveSprBuf(void *pv, ...);
extern "C" {
M4 CalculateEmitdvMatrix__FP6EMITDVfP7MATRIX4(void *p);
void FlushCache(int);

void func_00201998(void);
int CblipeProjectBlipTransformAccel(M4 *pm, VU_FLOATM dt, char *a, char *b, char *c, int n, int d, VECTOR *pvMin, VECTOR *pvMax);

void RemoveBlip__FP4BLIP(Q *pq);
int sceDmaSync(void *pch, int mode, int timeout);
void ProjectBlipgTransformAccel(PX *p);
}
static inline void InitDli(DLI *pdli, DL *pdl)
{
    pdli->m_pdl = pdl;
    pdli->m_ppv = (void **)pdl;
    pdli->m_ibDle = pdl->ibDle;
    pdli->m_pdliNext = D_002DD2FC;
    D_002DD2FC = pdli;
}
static inline void *PvNextDli(DLI *pdli)
{
    void *pv = *pdli->m_ppv;
    pdli->m_ppv = (void **)((char *)pv + pdli->m_ibDle);
    return pv;
}
static inline VU_FLOATM VuFloatDt()
{
    VU_FLOATM v;
    __asm__("lw %0, %1" : "=r"(v.qw) : "m"(D_002DDED0[1]));
    return v;
}
static inline char *PvSprBuf(int i)
{
    return D_003DD2C0[D_002C3C5C].a[i].pv;
}
static inline void Kick(Q *pq)
{
    BUF *pb = &pq->abuf[pq->i];
    LoadSprBuf(pq->c * 16, pb, pq->c * 16, pb->ab2, pq->c * 16, pb->ab3, 0, 0);
}
static inline qword QwMid(const qword &a, const qword &b, const VU_FLOAT &g)
{
    qword qw;
    __asm__("vmulax.xyzw $ACC, %1, %3x\n\tvmaddx.xyzw %0, %2, %3x" : "=j"(qw) : "j"(a), "j"(b), "j"(g.qw));
    return qw;
}
static inline qword QwSub(const qword &qw0, const qword &qw1)
{
    qword qw;
    __asm__("vsub.xyzw %0, %1, %2" : "=j"(qw) : "j"(qw0), "j"(qw1));
    return qw;
}
static inline qword QwLength(const qword &v)
{
    qword qw;
    qword vfOne;
    __asm__("vmul.xyz %0, %2, %2\n\tvaddw.x %1, $vf0, $vf0w\n\tvadday.x $ACC, %0, %0y\n\tvmaddz.x %0, %1, %0z"
            : "=j"(qw), "=j"(vfOne) : "j"(v));
    __asm__("vsqrt $Q, %0x\n\tvwaitq\n\tvaddq.x %0, $vf0, $Q" : "=j"(qw) : "0"(qw));
    return qw;
}
static inline float GFromQw(const qword &qw)
{
    float g;
    __asm__("mtc1 %1, %0" : "=f"(g) : "r"(qw)); /* mtc1 from a qword: gcc emits the qmfc2 */
    return g;
}
/** @sly2-annot addr=0x139AB8
 * Purpose: VU-accelerated particle advance with bounds
 * Info: proposed name ProjectBlipgTransformAccel (T1); TU blip
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r2b_rest+verdicts_A1
 * See: docs/engine/particles-effects.md#6-key-functions,
 *   docs/reference/functions/particles-effects.md#blip-0x138f08-0x13a9e8
 * @end-sly2-annot */
void ProjectBlipgTransformAccel(PX *p)
{
    VECTOR vMin = {{{FLT_MAX, FLT_MAX, FLT_MAX}}};
    VECTOR vMax = {{{-FLT_MAX, -FLT_MAX, -FLT_MAX}}};
    M4 m;
    m = CalculateEmitdvMatrix__FP6EMITDVfP7MATRIX4(p->x170);
    {
    DLI dli;
    Q *pq;
    InitDli(&dli, &p->dl);
    pq = (Q *)PvNextDli(&dli);
    {
        BUF *pb = &pq->abuf[pq->i];
        FlushCache(0);
        LoadSprBuf(pq->c * 16, pb, pq->c * 16, pb->ab2, pq->c * 16, pb->ab3, 0, 0);
    }
    while (pq) {
        Q *pqNext;
        int n, i;
        func_00201998();
        pqNext = (Q *)PvNextDli(&dli);
        if (pqNext) Kick(pqNext);
        n = CblipeProjectBlipTransformAccel(&m, VuFloatDt(), PvSprBuf(0), PvSprBuf(1), PvSprBuf(2), pq->c, p->x43C, &vMin, &vMax);
        i = (pq->i + 1) & 1;
        {
            BUF *pb = &pq->abuf[i];
            SaveSprBuf(pb, pb->ab2, pb->ab3, 0);
        }
        pq->c -= n;
        p->c444 -= n;
        if (pq->c <= 0) RemoveBlip__FP4BLIP(pq);
        else pq->i = i;
        pq = pqNext;
    }
    sceDmaSync(D_002DE59C, 0, 0);
    if (p->c444 <= 0) {
        p->f9();
    } else {
        qword c = QwMid(vMin.q, vMax.q, VU_FLOAT(0.5f));
        p->vF0 = c;
        float g = GFromQw(QwLength(QwSub(c, vMax.q)));
        p->g100 = g + p->g120;
    }
    D_002DD2FC = dli.m_pdliNext;
    }
}
