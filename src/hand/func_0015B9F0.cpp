// func_0015B9F0: emitter particle transform (Claude agent, lane E20). The clock time is read into a local BEFORE
// the ptimes->at[i] load, so loop.c sees a 1-insn life for %hi(D_002DDED0) and doesn't hoist it.
typedef int qword __attribute__((mode(TI)));
struct M4 { qword r[4]; };
struct BUF { qword *apos; qword *av; float *ag; float *an; int f10; };
struct SAV { int x0; };
struct EMITB { char pad[8]; SAV *psav; char pad2[0x18 - 0xC]; int k18; int f1C; };
class ALO;
struct VT { char pad[0x100]; short d; short pad2; void *(*pfn)(ALO *, M4 *); };
class ALO { public: char pad[0xC]; VT *pvt; int x10; char pad2[0xE4 - 0x14]; EMITB *pemitb; };
struct EO { char pad[8]; ALO *palo; };
struct EMITTER { char pad[8]; EO *peo; };
struct TIMES { char pad[0xC]; float *at; };
struct CLOCK { float t; };
extern "C" {
extern CLOCK D_002DDED0;
extern float D_002A5150;
void func_0015B7E8(int c, EMITB *pemitb, BUF *pb);
int func_0011D4D0(float g1, float g2, float gEps);
void func_002309E8(void *p, int n, M4 *pm0, float t, M4 *pm1);
void func_0011C168(M4 *pmOut, M4 *pmA, M4 *pmB);
void func_0015B9F0(int c, EMITTER *pemit, TIMES *ptimes, BUF *pb);
}
struct VU_FLOAT { qword q; VU_FLOAT(float f) { __asm__("qmtc2.ni %1, %0" : "=j"(q) : "r"(f)); } };
static inline qword MulAffine(const M4 &m, qword v)
{
    qword r;
    __asm__("vmulax.xyz $ACC, %1, %5x\n\tvmadday.xyz $ACC, %2, %5y\n\tvmaddaz.xyz $ACC, %3, %5z\n\tvmaddw.xyz %0, %4, $vf0w"
            : "=j"(r) : "j"(m.r[0]), "j"(m.r[1]), "j"(m.r[2]), "j"(m.r[3]), "j"(v));
    return r;
}
static inline qword MulRot(const M4 &m, qword v)
{
    qword r;
    __asm__("vmulax.xyz $ACC, %2, %1x\n\tvmadday.xyz $ACC, %3, %1y\n\tvmaddz.xyz %0, %4, %1z"
            : "=j"(r) : "j"(v), "j"(m.r[0]), "j"(m.r[1]), "j"(m.r[2]));
    return r;
}
static inline qword Add(qword a, qword b) { qword d; __asm__("vadd.xyzw %0, %1, %2" : "=j"(d) : "j"(a), "j"(b)); return d; }
static inline qword Scale(qword v, VU_FLOAT s) { qword d; __asm__("vmulx.xyzw %0, %1, %2x" : "=j"(d) : "j"(v), "j"(s.q)); return d; }

/** @sly2-annot addr=0x15B9F0
 * Purpose: For spline-sink emitters (kind 2) computes the extra per-particle transform data passed to
 *   EmitBlips
 * Info: proposed name OriginateSplineSinkParticles (T1); TU emitter
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: r3_1539A8
 * See: docs/reference/functions/particles-effects.md#emitter-0x159118-0x15cd68
 * @end-sly2-annot */
void func_0015B9F0(int c, EMITTER *pemit, TIMES *ptimes, BUF *pb)
{
    ALO *palo = pemit->peo->palo;
    SAV *psav = palo->pemitb->psav;
    int saved = psav->x0;
    int i;
    float tPrev;
    void *pv;
    M4 m;
    M4 mA;
    M4 mB;
    M4 mBase;
    M4 mWorld;
    if (!saved)
        psav->x0 = palo->x10;
    pb->f10 = 0;
    func_0015B7E8(c, palo->pemitb, pb);
    palo->pemitb->f1C = 0;
    tPrev = D_002A5150;
    pv = palo->pvt->pfn((ALO *)((char *)palo + palo->pvt->d), &mWorld);
    mBase = mWorld;
    for (i = 0; i < c; i++) {
        float tNow = D_002DDED0.t;
        float t = ptimes->at[i] - tNow;
        int k = palo->pemitb->k18;
        if (!func_0011D4D0(t, tPrev, 0.0001f)) {
            M4 mT;
            tPrev = t;
            func_002309E8(pv, 0, &m, t, 0);
            func_0011C168(&mT, &m, &mBase);
            m = mT;
            if (k != 3) {
                func_002309E8(pv, 0, &mA, t, &mB);
                func_0011C168(&mT, &mA, &mWorld);
                mA = mT;
                func_0011C168(&mT, &mB, &mWorld);
                mB = mT;
            }
        }
        pb->apos[i] = MulAffine(m, pb->apos[i]);
        if (k != 3) {
            M4 *pmA = &mA;
            pb->av[i] = MulRot(*pmA, pb->av[i]);
            if (k != 2) {
                qword v[1];
                if (k == 1)
                    v[0] = pb->apos[i];
                else
                    v[0] = mA.r[3];
                pb->av[i] = Add(pb->av[i], MulAffine(mB, v[0]));
            }
        }
        qword vv = pb->av[i];
        pb->av[i] = Scale(vv, VU_FLOAT(-1.0f));
    }
    palo->pemitb->psav->x0 = saved;
}
