// emitter.cpp -- particle emitters (EMITTER : XFM) and explosions (EXPL/EXPLG/EXPLS/EXPLO): emitter blocks (EMITB),
// origins (EMITO) and velocities (EMITV), emission of particles / blips / rips, the emitter update and pause, and
// the explosion groups. TU 0x159118..0x15CD68 (54 functions with the linkonce copies at 0x258620..0x2599C8,
// address order).
//
// Types: the file-scope EMITTER is a view of the fields the m2c bodies read (sly2/emitter.h's EMITTER pads them;
// emitter.h is not included because its EMITV is a different record from the 0x70-byte EMITV defaults below).
// The hand-written per-function files keep their views in namespaces named after each function (as jt.cpp does);
// their C++-named definitions are C-linkage definitions of their literal symbols there (the namespace would change
// the mangling), as in wave 9.
// The static initializer (func_0015CAD8, stub func_0015CD48) is generated from the two emitter defaults and the six
// slot pools below.
// SLY2_ALIAS: __static_initialization_and_destruction_0=func_0015CAD8
// SLY2_ALIAS: _GLOBAL_$I$*=func_0015CD48

#include "sly2/basic.h"
#include "sly2/xfm.h"
#include "sly2/slotheap.h"

/* The emitter defaults (0x15CAD8, the static initializer). D_002DE5B0 / D_002DE5F0 are the EMITO / EMITV defaults
   that func_00159118 (InitEmitb) copies into EMITB+0x10 / +0x50 (Sly 1's InitEmitb builds the same two objects as
   guarded local statics). Their initializers hold the non-constant vectors D_002A2DD0 (0,0,0), D_002A2DA0 (1,0,0)
   and D_002A2DC0 (0,0,1), so g++ builds each one in a memset stack temp and block-copies it.
   The three unions are what matched it: store_constructor emits a (clobber (mem:BLK)) for every union initializer.
   - EMITV+0x60 is a CLQ (Sly 1 util.h: union of {g0, g1, g2, gUnused} and ag[4]) = {1, 0, -1}. Its clobber is a
     scheduler barrier, so its two stores issue last and 1.0 / -1 die only there (the game's store order).
   - the union at EMITO+0x20 sits inside the live ranges of the first sentry %hi's only, which breaks their
     global-alloc ties (s4/s3, s6/s5), and it fixes the D_002A2DD0 / D_002A2DA0 v0/v1 choice.
   - the union at EMITV+0x00 fixes the FPU numbers of 1.0 / -1500, the 200 / loop-end registers and the s1/a2 order.
   (Lilly's agent, T1656; the per-function file needed the gnu-reorder-db assembler mode and an asm label, which
   the merged file does not.) */
namespace EMITDEF {
union CLQU {
    struct { float g0, g1, g2, gUnused; };
    float ag[4];
};
union QWV { qword qw; float ag[4]; };
union G4V { struct { float g0, g4, g8, gC; }; float ag[4]; };
struct EMITO {
    int x0[4];
    int x10;
    int x14[3];
    QWV v20;
    qword v30;
};
struct EMITV {
    G4V g0;
    int x10, x14;
    float x18;
    int x1C;
    qword v20;
    int x30;
    int x34;
    float x38, x3C;
    int x40[8];
    CLQU clq60;
};
typedef char check_emito[(sizeof(EMITO) == 0x40) ? 1 : -1];
typedef char check_emitv[(sizeof(EMITV) == 0x70) ? 1 : -1];
}
extern "C" qword D_002A2DD0, D_002A2DA0, D_002A2DC0;
EMITDEF::EMITO D_002DE5B0 = { {0, 0, 0, 0}, -1, {0, 0, 0}, {D_002A2DD0}, D_002A2DA0 };
EMITDEF::EMITV D_002DE5F0 = { {{750.0f, 750.0f, -1.0f, 1.0f}}, 0, 0, -1500.0f, 0, D_002A2DC0, 0, 200, 1.0f, 1.0f,
                              {0, 0, 0, 0, 0, 0, 0, 0}, {{1.0f, 0.0f, -1.0f}} };

// 0x15CAD8 static initializer (after the defaults) and its _GLOBAL_$I$ stub func_0015CD48: the six slot pools.
SLY2_INSTANTIATE_SLOT_POOLS()

struct RIPG;
struct BLIPG;
struct SFX;
/* The emitter block as the m2c bodies read it: share count, and at +0x8 a pointer to the parent's id. */
struct EMITB
{
    int cref;
    int kv;
    int *poidParent;        /* +0x08 (HYPOTHESIS: by func_00159968) */
};
/* EMITTER (CID 0x1D : XFM, cb 0x160): the fields the m2c bodies use; names follow the hand-written views. */
struct EMITTER : public XFM
{
    int fDirty;             /* +0xE0 */
    EMITB *pemitb;          /* +0xE4 */
    char padE8[8];
    int k;                  /* +0xF0 emission kind (3: rate per second) */
    int c;                  /* +0xF4 */
    LM lmRate;              /* +0xF8 */
    char pad100[0x114 - 0x100];
    int fAutoPause;         /* +0x114 */
    char pad118[0x130 - 0x118];
    float tNext;            /* +0x130 */
    char pad134[0x148 - 0x134];
    float tUnpause;         /* +0x148 */
    RIPG *pripg;            /* +0x14C */
    BLIPG *pblipg;          /* +0x150 */
    char pad154[0x160 - 0x154];
};
typedef char assert_EMITTER_size[sizeof(EMITTER) == 0x160 ? 1 : -1];
/* EXPLG (an explosion group, : EXPL : XFM): its children. */
struct EXPLG : public XFM
{
    int cplo;               /* +0xE0 */
    LO **aplo;              /* +0xE4 */
};
/* The object of func_0015C650 (class unknown): a lazily created SFX at +0xF0. */
struct SFXHOLDER : public XFM
{
    char padE0[0x10];
    SFX *psfx;              /* +0xF0 */
};

extern SW D_003DD490;       /* g_sw */
extern "C" {
extern float D_002A514C;    /* "indefinitely": a time far in the future */
extern CLOCK D_002DDED0;    /* g_clock */
void func_001C4848(SW *psw, EMITTER *pemitter);
void SetLoParent_1A58D0(LO *plo, SO *psoParent, int n);
SO *PsoFromLo(int oid);
void SetBlipgEmitb(BLIPG *pblipg, EMITB *pemitb);
SFX *NewSfx();
void CloneXfm(XFM *pxfm, XFM *pxfmBase);
}
void SetRipgEmitb(RIPG *pripg, EMITB *pemitb);
float GRandInRange(float gMin, float gMax);

namespace M_func_00159118 { extern "C" {
typedef int qword __attribute__((mode(TI)));
typedef struct M4 { qword aq[4]; } M4;
typedef struct B7 { qword aq[7]; } B7;
extern M4 D_002DE5B0;
extern B7 D_002DE5F0;
struct S { int a; int b; int pad[2]; M4 m; B7 b7; char pad2[0x100 - 0xC0]; };
void *func_002333B4(void *pv, int b, int cb);
/** @sly2-annot addr=0x159118
 * Purpose: Zeroes an EMITB and installs the default EMITO/EMITV objects (D_002DE5B0/D_002DE5F0)
 * Info: proposed name InitEmitb (T1); TU emitter
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r3_1539A8
 * See: docs/engine/core-library.md#6-source-files, docs/engine/particles-effects.md#5-source-files,
 *   docs/engine/particles-effects.md#6-key-functions
 * @end-sly2-annot */
void func_00159118(struct S *p)
{
    func_002333B4(p, 0, sizeof(struct S));
    p->a = 1;
    p->b = -1;
    p->m = D_002DE5B0;
    p->b7 = D_002DE5F0;
}
} } // namespace M_func_00159118

namespace M_InitEmitter { extern "C" {
struct S { char pad[0x40]; unsigned int fl; char pad2[0xF0 - 0x44]; int nF0; int nF4; float gF8; float gFC; char pad3[0x118 - 0x100]; int n118; char dl[0x148 - 0x11C]; float g148; };
void InitXfm(void *p);
void InitDl__FP2DLi(void *pdl, int ib);
void SetLoBusyk(void *p, int n);
/** @sly2-annot addr=0x1591B0
 * Purpose: EMITTER::Init: XFM init, default indices -1, g=10.0, ClearDl of particle list, register for
 *   update, flag 2
 * Info: proposed name InitEmitter (T1); class EMITTER slot7; TU emitter
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r3_1539A8
 * See: docs/engine/particles-effects.md#1-emitters-emittercpp-0x159118-0x15cd68,
 *   docs/engine/particles-effects.md#6-key-functions,
 *   docs/reference/functions/particles-effects.md#emitter-0x159118-0x15cd68
 * @end-sly2-annot */
void InitEmitter(struct S *p)
{
    unsigned int v;
    InitXfm(p);
    p->nF4 = -1;
    p->nF0 = -1;
    p->n118 = -1;
    p->gFC = 10.0f;
    p->gF8 = 10.0f;
    p->g148 = -1.0f;
    InitDl__FP2DLi(p->dl, 0x128);
    SetLoBusyk(p, 0);
    v = p->fl;
    if ((v ^ 1) & 1) *(int *)v |= 2;
    else p->fl = v | 2;
}
} } // namespace M_InitEmitter

namespace M_LoadEmitmeshFromBrx__FP8EMITMESHP18CBinaryInputStream {
typedef int qword __attribute__((mode(TI)));
struct AL { int cb; int cbAlign; AL(int c, int a) { cb = c; cbAlign = a; } };
struct CBinaryInputStream;
struct TRI { int aipos[3]; float sArea; };
struct EMITMESH { int cpos; qword *apos; int ctri; TRI *atri; qword *anormal; float sArea; int pad18[2]; qword v20; };
extern "C" {
int U16Read__18CBinaryInputStream(CBinaryInputStream *pbis);
void ReadVector__18CBinaryInputStreamP6VECTOR(CBinaryInputStream *pbis, qword *pv);
float F32Read__18CBinaryInputStream(CBinaryInputStream *pbis);
void *func_001AE1D0(AL *pal);
void *func_001AE2A0(AL *pal);
}
extern "C" { extern qword D_002A2DC0; }
static inline void *PvAlloc(int cb, int cbAlign) { AL al(cb, cbAlign); return func_001AE1D0(&al); }
static inline void *PvAllocClear(int cb, int cbAlign) { AL al(cb, cbAlign); return func_001AE2A0(&al); }
static inline qword Sub(qword a, qword b) { qword d; __asm__("vsub.xyzw %0, %1, %2" : "=j"(d) : "j"(a), "j"(b)); return d; }
static inline qword Add(qword a, qword b) { qword d; __asm__("vadd.xyzw %0, %1, %2" : "=j"(d) : "j"(a), "j"(b)); return d; }
static inline qword Cross(qword a, qword b)
{
    qword d;
    __asm__("vopmula.xyz ACC, %1, %2\n\tvopmsub.xyz %0, %2, %1" : "=j"(d) : "j"(a), "j"(b));
    return d;
}
static __inline__ qword VLen(qword c)
{
    qword l, one;
    __asm__("vmul.xyz %0, %2, %2\n\tvaddw.x %1, $vf0, $vf0w\n\tvadday.x ACC, %0, %0y\n\tvmaddz.x %0, %1, %0z" : "=&j"(l), "=&j"(one) : "j"(c));
    {
    qword r;
    __asm__("vsqrt Q, %1x\n\tvwaitq\n\tvaddq.x %0, $vf0, Q" : "=j"(r) : "j"(l));
    return r;
    }
}
static __inline__ void VNormalize(qword *pr, qword c, qword k)
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
        __asm__ __volatile__("vdiv Q, $vf0w, %2x\n\tvwaitq\n\tvmulq.xyzw %0, %1, Q" : "=j"(t) : "j"(c), "j"(l));
    }
    *pr = t;
}
/** @sly2-annot addr=0x159240
 * Purpose: not written up yet; only the symbol name (pre-existing / Sly 1-derived) describes it.
 * Info: proposed name LoadEmitmeshFromBrx__FP8EMITMESHP18CBinaryInputStream (EXISTING); TU emitter
 * Notes: level_parser CONFIRMED-parse: payload layout followed by the parser, 100%% of bytes in 4 levels
 * Confidence: name high [EXISTING 0.75]; purpose not reviewed - evidence: pre-existing symbol in
 *   symbol_addrs.txt
 * See: docs/engine/particles-effects.md#6-key-functions,
 *   docs/reference/functions/particles-effects.md#emitter-0x159118-0x15cd68
 * @end-sly2-annot */
extern "C" void LoadEmitmeshFromBrx__FP8EMITMESHP18CBinaryInputStream(EMITMESH *pemitmesh, CBinaryInputStream *pbis)
{
    int i;
    pemitmesh->cpos = U16Read__18CBinaryInputStream(pbis);
    pemitmesh->apos = (qword *)PvAlloc(pemitmesh->cpos * 16, 16);
    pemitmesh->anormal = (qword *)PvAllocClear(pemitmesh->cpos * 16, 16);
    for (i = 0; i < pemitmesh->cpos; i++)
        ReadVector__18CBinaryInputStreamP6VECTOR(pbis, &pemitmesh->apos[i]);
    pemitmesh->ctri = U16Read__18CBinaryInputStream(pbis);
    pemitmesh->atri = (TRI *)PvAlloc(pemitmesh->ctri * 16, 16);
    pemitmesh->sArea = 0.0f;
    for (int itri = 0; itri < pemitmesh->ctri; itri++) {
        TRI *ptri = &pemitmesh->atri[itri];
        for (i = 0; i < 3; i++)
            ptri->aipos[i] = U16Read__18CBinaryInputStream(pbis);
        ptri->sArea = F32Read__18CBinaryInputStream(pbis);
        pemitmesh->sArea += ptri->sArea;
        qword *apos = pemitmesh->apos;
        qword p0 = apos[ptri->aipos[0]];
        qword normal = Cross(Sub(apos[ptri->aipos[1]], p0), Sub(apos[ptri->aipos[2]], p0));
        for (i = 0; i < 3; i++) {
            qword *pn = &pemitmesh->anormal[ptri->aipos[i]];
            *pn = Add(*pn, normal);
        }
    }
    ReadVector__18CBinaryInputStreamP6VECTOR(pbis, &pemitmesh->v20);
    for (i = 0; i < pemitmesh->cpos; i++)
        VNormalize(&pemitmesh->anormal[i], pemitmesh->anormal[i], D_002A2DC0);
}
} // namespace M_LoadEmitmeshFromBrx__FP8EMITMESHP18CBinaryInputStream

/** @sly2-annot addr=0x1594D8
 * Purpose: Forwards to 0x1C4848(&g_sw, p) (SW-level registration of the emitter)
 * Info: TU emitter
 * Notes: no usable identifier; purpose only
 * Confidence: low (draft) [T2 0.25] - evidence: static analysis; sources: r3_1539A8+verdicts_A1
 * See: docs/engine/particles-effects.md#1-emitters-emittercpp-0x159118-0x15cd68
 * @end-sly2-annot */
extern "C" void func_001594D8(EMITTER *pemitter)
{
    func_001C4848(&D_003DD490, pemitter);
}

namespace M_PostEmitterLoad__FP7EMITTER { extern "C" {
struct K { short a; short b; int c; };
struct LM { float gMin; float gMax; };
struct BOX { float xMin, yMin, zMin, pad; float xMax, yMax, zMax; };
struct EMITO { void *palo; };
struct EMITB { int n0; int pad4; struct EMITO *pemito; int padC; int kind; char pad14[0x20 - 0x14]; int n20; char pad24[0x30 - 0x24]; struct BOX *pbox; };
struct VTE { short delta; short pad; void (*pfn)(void *, void *); };
struct EMITTER {
    char pad0[0xC]; struct VTE *pvt; char pad10[0x40 - 0x10]; int grf40; char pad44[0xE0 - 0x44];
    int nE0; struct EMITB *pemitb; char padE8[0xF0 - 0xE8];
    int kind; int nF4; struct LM lmF8; int n100; float g104; struct LM lm108; float g110; int n114; int oid118;
    char pad11C[0x130 - 0x11C]; float g130; float g134; float g138; float g13C; float g140;
};
struct ALLOCREQ { int cb; int kind; };
extern char D_003DD490[];
struct LONGU { long l; } __attribute__((packed));
extern const struct LONGU D_002A5140;
extern float D_002A5148;
void func_002586D8(struct EMITTER *p);
void PostSwCallback(void *pg, struct EMITTER *p, long k, int n);
float GRandInRange__Fff(float gMin, float gMax);
void PauseEmitterIndefinite__FP7EMITTER(struct EMITTER *p);
void PauseEmitter__FP7EMITTERf(struct EMITTER *p, float g);
struct EMITTER *PloFindSwObject__FP2SWi3OIDP2LO(void *pg, int kind, int oid, int n);
struct EMITTER *PloNew__F3CIDP2SO3OIDi(int cid, void *p, int oid, int n);
void *func_001AE238(struct ALLOCREQ *preq, void *pv);
void SetLoBusyk(struct EMITTER *p, int n);
void *func_001A6318(struct EMITTER *p);
void AppendDlEntry__FP2DLPv(void *pv, struct EMITTER *p);

static __inline__ void *PvAllocCopy(int cb, void *pv)
{
    struct ALLOCREQ req;
    req.cb = cb;
    req.kind = 0x10;
    return func_001AE238(&req, pv);
}

static __inline__ int FPtr(int n) { return !(n & 1); }

/** @sly2-annot addr=0x159500
 * Purpose: not written up yet; only the symbol name (pre-existing / Sly 1-derived) describes it.
 * Confidence: mid (draft) - evidence: pre-existing symbol name only; behaviour not reviewed
 * See: docs/engine/particles-effects.md#1-emitters-emittercpp-0x159118-0x15cd68,
 *   docs/reference/functions/particles-effects.md#emitter-0x159118-0x15cd68
 * @end-sly2-annot */
void PostEmitterLoad__FP7EMITTER(struct EMITTER *pemitter)
{
    func_002586D8(pemitter);
    if (pemitter->pemitb->pemito->palo)
        PostSwCallback(D_003DD490, pemitter, D_002A5140.l, 0);
    if (pemitter->kind == -1) {
        if (pemitter->g104 != 0.0f || pemitter->n114)
            pemitter->kind = 3;
        else if (pemitter->g110 != 0.0f)
            pemitter->kind = 1;
        else
            pemitter->kind = 0;
    }
    switch (pemitter->kind) {
    case 2:
        pemitter->g138 = D_002A5148;
        pemitter->g130 = GRandInRange__Fff(pemitter->lmF8.gMin, pemitter->lmF8.gMax);
        if (pemitter->n114)
            PauseEmitterIndefinite__FP7EMITTER(pemitter);
        else if (GRandInRange__Fff(0.0f, 1.0f) <= pemitter->g104)
            PauseEmitter__FP7EMITTERf(pemitter, GRandInRange__Fff(pemitter->lm108.gMin, pemitter->lm108.gMax));
        break;
    case 3:
        pemitter->g138 = D_002A5148;
        pemitter->g130 = GRandInRange__Fff(pemitter->lmF8.gMin, pemitter->lmF8.gMax) * 60.0f;
        if (pemitter->n114)
            PauseEmitterIndefinite__FP7EMITTER(pemitter);
        else if (GRandInRange__Fff(0.0f, 1.0f) <= pemitter->g104)
            PauseEmitter__FP7EMITTERf(pemitter, GRandInRange__Fff(pemitter->lm108.gMin, pemitter->lm108.gMax));
        break;
    case 0:
    case 1:
        pemitter->g134 = pemitter->g138 = 1.0f;
        pemitter->g130 = GRandInRange__Fff(pemitter->lmF8.gMin, pemitter->lmF8.gMax);
        break;
    }
    pemitter->g13C = 1.0f;
    if (pemitter->pemitb->kind == 1) {
        struct BOX *pbox = pemitter->pemitb->pbox;
        if (pemitter->pemitb->pemito->palo) {
            float dx = __builtin_fabsf(pbox->xMax - pbox->xMin);
            float dy = __builtin_fabsf(pbox->yMax - pbox->yMin);
            float dz = __builtin_fabsf(pbox->zMax - pbox->zMin);
            float g;
            if (dy < dx && dz < dx)
                g = dx;
            else if (dz < dy)
                g = dy;
            else
                g = dz;
            pemitter->g140 = g * 0.5f;
        }
        if (pemitter->n100) {
            if (pemitter->g110 != 0.0f)
                pemitter->g13C = (pbox->xMax - pbox->xMin) * (pbox->yMax - pbox->yMin) * (pbox->zMax - pbox->zMin) * 1e-6f;
            else
                pemitter->g13C = (pbox->yMax - pbox->yMin) * (pbox->zMax - pbox->zMin) * 1e-4f;
        }
    }
    if (pemitter->oid118 != -1) {
        struct EMITTER *pemitterNew = PloFindSwObject__FP2SWi3OIDP2LO(D_003DD490, 5, pemitter->oid118, 0);
        if (!pemitterNew) {
            struct VTE *pvte;
            void *pthis;
            pemitterNew = PloNew__F3CIDP2SO3OIDi(0x1D, 0, pemitter->oid118, -1);
            pemitterNew->pemitb = (__typeof__(pemitterNew->pemitb))PvAllocCopy(0x100, pemitter->pemitb);
            pemitterNew->pemitb->n0 = 1;
            if (pemitter->pemitb->pemito->palo) {
                pemitterNew->pemitb->pemito = (__typeof__(pemitterNew->pemitb->pemito))PvAllocCopy(0x14, pemitter->pemitb->pemito);
                pemitterNew->pemitb->n20 = -1;
                pemitterNew->pemitb->pemito->palo = 0;
            }
            pemitterNew->kind = pemitter->kind;
            pemitterNew->nF4 = pemitter->nF4;
            pemitterNew->lmF8 = pemitter->lmF8;
            pemitterNew->n100 = pemitter->n100;
            pemitterNew->g104 = pemitter->g104;
            pemitterNew->lm108 = pemitter->lm108;
            pemitterNew->g110 = pemitter->g110;
            pemitterNew->n114 = pemitter->n114;
            SetLoBusyk(pemitterNew, 1);
            pvte = &pemitterNew->pvt[30];
            pthis = (char *)pemitterNew + pvte->delta;
            pvte->pfn(pthis, func_001A6318(pemitter));
        }
        pemitter->nE0 = 0;
        AppendDlEntry__FP2DLPv((char *)pemitterNew + 0x11C, pemitter);
        if (FPtr(pemitter->grf40))
            *(int *)pemitter->grf40 &= ~2;
        else
            pemitter->grf40 &= ~2;
    }
}
} } // namespace M_PostEmitterLoad__FP7EMITTER

/** @sly2-annot addr=0x159968
 * Purpose: Re-parents the emitter to the LO found via +0xE4 (0x1A58D0(p, 0x1A5BF0(oid), 0))
 * Info: TU emitter
 * Notes: no usable identifier; purpose only
 * Confidence: low (draft) [T2 0.25] - evidence: static analysis; sources: r3_1539A8
 * @end-sly2-annot */
extern "C" void func_00159968(EMITTER *pemitter)
{
    SetLoParent_1A58D0(pemitter, PsoFromLo(*pemitter->pemitb->poidParent), 0);
}

namespace M_HandleEmitterMessage__FP7EMITTER5MSGIDPv {
struct EMITTER { char pad[0x14C]; void *p14C; void *p150; };
enum MSGID { MSGID_0, MSGID_1 };
extern "C" { void UnsubscribeLoObject(void *pv, void *po); }
/** @sly2-annot addr=0x1599A8
 * Purpose: not written up yet; only the symbol name (pre-existing / Sly 1-derived) describes it.
 * Confidence: mid (draft) - evidence: pre-existing symbol name only; behaviour not reviewed
 * See: docs/engine/particles-effects.md#1-emitters-emittercpp-0x159118-0x15cd68,
 *   docs/reference/functions/particles-effects.md#emitter-0x159118-0x15cd68
 * @end-sly2-annot */
extern "C" void HandleEmitterMessage__FP7EMITTER5MSGIDPv(EMITTER *pemitter, MSGID msgid, void *pv)
{
    if (msgid == 1) {
        if (pv == pemitter->p14C) {
            UnsubscribeLoObject(pv, pemitter);
            pemitter->p14C = 0;
        } else if (pv == pemitter->p150) {
            UnsubscribeLoObject(pv, pemitter);
            pemitter->p150 = 0;
        }
    }
}
} // namespace M_HandleEmitterMessage__FP7EMITTER5MSGIDPv

/** @sly2-annot addr=0x159A08
 * Purpose: not written up yet; only the symbol name (pre-existing / Sly 1-derived) describes it.
 * Confidence: mid (draft) - evidence: pre-existing symbol name only; behaviour not reviewed
 * See: docs/reference/functions/particles-effects.md#emitter-0x159118-0x15cd68
 * @end-sly2-annot */
#ifndef SLY2_ENUM_ENSK
#define SLY2_ENUM_ENSK
enum ENSK { ENSK_Nil = -1, ENSK_Get = 0, ENSK_Set = 1 };
#endif
EMITTER *PemitterEnsureEmitter(EMITTER *pemitter, ENSK ensk)
{
    if (ensk == ENSK_Set) {
        pemitter->fDirty = ensk;
    }
    return pemitter;
}

namespace M_ModifyEmitterParticles__FP7EMITTER {
typedef int qword __attribute__((mode(TI)));
union V { qword q; struct { float x, y, z, w; } v; float a[4]; };
struct BLK { qword apos[2][0x84]; int ibuf; int cpart; int pad[2]; BLK *pblkNext; };
struct PRG { char pad[0x448]; BLK *pblkFirst; };
struct ALO;
struct EMITO { ALO *palo; };
struct EMITB { int pad0; int f4; EMITO *pemito; };
struct EMITTER { char pad[0xE4]; EMITB *pemitb; char pad1[0x140 - 0xE8]; float sBox; char pad2[0x150 - 0x144]; PRG *pprg; };
extern "C" {
qword func_001A62E8(ALO *palo);
float GModPositive__Fff(float g, float gMod);
}
static inline qword Sub(qword a, qword b) { qword d; __asm__("vsub.xyzw %0, %1, %2" : "=j"(d) : "j"(a), "j"(b)); return d; }
static inline qword Add(qword a, qword b) { qword d; __asm__("vadd.xyzw %0, %1, %2" : "=j"(d) : "j"(a), "j"(b)); return d; }
static inline void Wrap(float *pg, float &gMin, float &gMax, float &gBox, float *pdg)
{
    if (*pg < gMin || gMax < *pg) {
        *pdg = gMin + GModPositive__Fff(*pg - gMin, gBox) - *pg;
        *pg += *pdg;
    }
}
/** @sly2-annot addr=0x159A20
 * Purpose: not written up yet; only the symbol name (pre-existing / Sly 1-derived) describes it.
 * Confidence: mid (draft) - evidence: pre-existing symbol name only; behaviour not reviewed
 * See: docs/reference/functions/particles-effects.md#emitter-0x159118-0x15cd68
 * @end-sly2-annot */
extern "C" void ModifyEmitterParticles__FP7EMITTER(EMITTER *pemitter)
{
    float sBox = pemitter->sBox;
    if (sBox > 0.0001f) {
        V vecBox, posMin, posMax, dpos;
        volatile int unused;
        vecBox.v.z = sBox; vecBox.v.y = sBox; vecBox.v.x = sBox;
        if (pemitter->pemitb->pemito->palo) {
            posMin.q = Sub(func_001A62E8(pemitter->pemitb->pemito->palo), vecBox.q);
            posMax.q = Add(func_001A62E8(pemitter->pemitb->pemito->palo), vecBox.q);
        } else {
            qword m, k;
            __asm__("qmtc2.ni %1, %0" : "=j"(k) : "r"(-1.0f));
            dpos.q = k;
            __asm__("vmulx.xyzw %0, %1, %2x" : "=j"(m) : "j"(vecBox.q), "j"(k));
            posMin.q = m;
            posMax.q = vecBox.q;
        }
        float sBox2 = pemitter->sBox + pemitter->sBox;
        vecBox.v.z = sBox2; vecBox.v.y = sBox2; vecBox.v.x = sBox2;
        if (pemitter->pemitb->f4 == 0 && pemitter->pprg) {
            for (BLK *pblk = pemitter->pprg->pblkFirst; pblk; pblk = pblk->pblkNext) {
                qword *apos = pblk->apos[pblk->ibuf];
                for (int i = 0; i < pblk->cpart; i++) {
                    V *ppos = (V *)&apos[i];
                    Wrap(&ppos->v.x, posMin.v.x, posMax.v.x, vecBox.v.x, &dpos.v.x);
                    Wrap(&ppos->v.y, posMin.v.y, posMax.v.y, vecBox.v.y, &dpos.v.y);
                    Wrap(&ppos->v.z, posMin.v.z, posMax.v.z, vecBox.v.z, &dpos.v.z);
                }
            }
        }
    }
}
} // namespace M_ModifyEmitterParticles__FP7EMITTER

namespace M_UpdateEmitter__FP7EMITTERf {
typedef int qword __attribute__((mode(TI)));
struct ALO;
struct CLOCK { float t; float dt; };
extern "C" { extern CLOCK D_002DDED0; }
struct CM { char pad[0x1C]; float g1C; char pad2[0x90 - 0x20]; qword pos; };
extern "C" { extern CM D_002DDF40; }
struct EMITO { void *palo; int f4; };
struct EMITB { int pad0; int f4; EMITO *pemito; };
struct RIPG { char pad[0x15C]; int dl; };
struct PRG { char pad[0x444]; int cpart; };
struct EMITTER {
    char pad0[0x10]; int x10; char pad1[0x38 - 0x14]; float g38;
    char pad2[0xE0 - 0x3C]; int fChanged; EMITB *pemitb; char padE8[8]; int k; int c;
    float gF8, gFC; char pad3[0x104 - 0x100]; float g104, g108, g10C, g110; int f114; int x118;
    int dl11C; char pad4[0x128 - 0x120]; EMITTER *pNext;
    char pad5[0x130 - 0x12C]; float g130, g134, g138, g13C; char pad6[0x144 - 0x140]; float g144;
    char pad7[0x14C - 0x148]; RIPG *pripg; PRG *pprg;
};
struct EMITX { PRG **ppprg; RIPG **ppripg; EMITTER *pemitter; };
extern "C" {
void UpdateLo(EMITTER *p);
int FIsDlEmpty__FP2DL(int *pdl);
int func_001A61A0(EMITTER *p);
qword func_001A62E8(void *p);
float GRandInRange__Fff(float gMin, float gMax);
int CPvDl__FP2DL(int *pdl);
void *func_002333B4(void *pv, int b, int cb);
void EmitParticles__FiP5EMITBP5EMITG(int c, EMITB *pemitb, EMITX *px);
}
void OnEmitterValuesChanged(EMITTER *p) __asm__("OnEmitterValuesChanged__FP7EMITTER");
int FPausedEmitter(EMITTER *p) __asm__("FPausedEmitter__FP7EMITTER");
void PauseEmitter(EMITTER *p, float dt) __asm__("PauseEmitter__FP7EMITTERf");
void PauseEmitterIndefinite(EMITTER *p) __asm__("PauseEmitterIndefinite__FP7EMITTER");
void ModifyEmitterParticles(EMITTER *p) __asm__("ModifyEmitterParticles__FP7EMITTER");

static inline float Dist2(qword a, qword b)
{
    qword d, t;
    __asm__("vsub.xyzw %0, %1, %2" : "=j"(d) : "j"(a), "j"(b));
    __asm__("vmul.xyz %0, %0, %0\n\tvaddw.x %1, $vf0, $vf0w\n\tvadday.x $ACC, %0, %0y\n\tvmaddz.x %0, %1, %0z" : "+j"(d), "=&j"(t));
    float g;
    __asm__("qmfc2.ni %0, %1" : "=r"(g) : "j"(d));
    return g;
}
static inline int FLess(float a, float b) { return a < b; }

/** @sly2-annot addr=0x159C80
 * Purpose: not written up yet; only the symbol name (pre-existing / Sly 1-derived) describes it.
 * Confidence: mid (draft) - evidence: pre-existing symbol name only; behaviour not reviewed
 * See: docs/engine/particles-effects.md#1-emitters-emittercpp-0x159118-0x15cd68,
 *   docs/reference/functions/particles-effects.md#emitter-0x159118-0x15cd68
 * @end-sly2-annot */
extern "C" void UpdateEmitter__FP7EMITTERf(EMITTER *p, float dt)
{
    if (p->fChanged)
        OnEmitterValuesChanged(p);
    UpdateLo(p);
    if (p->x118 != -1)
        return;
    if (!FIsDlEmpty__FP2DL(&p->dl11C)) {
        EMITTER *pe;
        for (pe = (EMITTER *)p->dl11C; pe; pe = pe->pNext) {
            if (!func_001A61A0(pe))
                continue;
            if (!pe->x10)
                goto LActive;
            float r = pe->g38 * D_002DDF40.g1C;
            qword pos = func_001A62E8(pe);
            r = r * r;
            if (FLess(Dist2(pos, D_002DDF40.pos), r))
                goto LActive;
        }
        return;
    } else if (p->x10) {
        float r = p->g38 * D_002DDF40.g1C;
        qword pos = func_001A62E8(p);
        r = r * r;
        if (FLess(r, Dist2(pos, D_002DDF40.pos)))
            return;
    }
LActive:
    if (FPausedEmitter(p))
        return;

    int c = 0;
    switch (p->k) {
    case 3: {
        float g = p->g144 + p->g130 * p->g13C * D_002DDED0.dt;
        c = (int)g;
        p->g144 = g - (float)c;
        if (1.0f < (float)c) {
            int cLeft = p->c;
            if (cLeft >= 0) {
                if (cLeft <= c)
                    c = cLeft;
                p->c = cLeft - c;
            }
            if (p->f114) {
                PauseEmitterIndefinite(p);
                p->g130 = GRandInRange__Fff(p->gF8, p->gFC) * 60.0f;
            } else if (GRandInRange__Fff(0.0f, 1.0f) <= p->g104) {
                PauseEmitter(p, GRandInRange__Fff(p->g108, p->g10C));
                p->g130 = GRandInRange__Fff(p->gF8, p->gFC) * 60.0f;
            }
        }
        break;
    }
    case 2: {
        c = (int)p->g130;
        int cLeft = p->c;
        if (cLeft >= 0) {
            if (cLeft <= c)
                c = cLeft;
            p->c = cLeft - c;
        }
        if (p->f114)
            PauseEmitterIndefinite(p);
        else if (GRandInRange__Fff(0.0f, 1.0f) <= p->g104)
            PauseEmitter(p, GRandInRange__Fff(p->g108, p->g10C));
        p->g130 = GRandInRange__Fff(p->gF8, p->gFC);
        break;
    }
    case 0: {
        float g = p->g144 + p->g130 * p->g13C * D_002DDED0.dt;
        c = (int)g;
        p->g144 = g - (float)c;
        if (c != 0) {
            int cLeft = p->c;
            if (cLeft >= 0) {
                if (cLeft <= c)
                    c = cLeft;
                p->c = cLeft - c;
            }
        }
        break;
    }
    case 1: {
        int cWant = (int)(p->g110 * p->g13C);
        int cCur;
        if (!p->pemitb->f4)
            cCur = p->pprg ? p->pprg->cpart : 0;
        else
            cCur = p->pripg ? CPvDl__FP2DL(&p->pripg->dl) : 0;
        int d = cWant - cCur;
        c = 0;
        int f = 0 < d;
        if (f)
            c = d;
        break;
    }
    }

    if (c) {
        int unused[4];
        EMITX x;
        func_002333B4(&x, 0, sizeof(x));
        x.pemitter = p;
        x.ppprg = &p->pprg;
        x.ppripg = &p->pripg;
        EMITO *pemito = p->pemitb->pemito;
        void *palo = pemito->palo;
        if (!palo && !pemito->f4)
            pemito->palo = p;
        EmitParticles__FiP5EMITBP5EMITG(c, p->pemitb, &x);
        p->pemitb->pemito->palo = palo;
    }

    if (D_002DDED0.t >= p->g138) {
        p->g130 = GRandInRange__Fff(p->gF8, p->gFC);
        p->g138 = D_002DDED0.t + p->g134;
    }
    ModifyEmitterParticles(p);
}
} // namespace M_UpdateEmitter__FP7EMITTERf

/** @sly2-annot addr=0x15A138
 * Purpose: not written up yet; only the symbol name (pre-existing / Sly 1-derived) describes it.
 * Confidence: mid (draft) - evidence: pre-existing symbol name only; behaviour not reviewed
 * See: docs/reference/functions/particles-effects.md#emitter-0x159118-0x15cd68
 * @end-sly2-annot */
void PauseEmitter(EMITTER *pemitter, float dt)
{
    pemitter->tUnpause = D_002DDED0.t + dt;
}

/** @sly2-annot addr=0x15A150
 * Purpose: not written up yet; only the symbol name (pre-existing / Sly 1-derived) describes it.
 * Confidence: mid (draft) - evidence: pre-existing symbol name only; behaviour not reviewed
 * See: docs/reference/functions/particles-effects.md#emitter-0x159118-0x15cd68
 * @end-sly2-annot */
void PauseEmitterIndefinite(EMITTER *pemitter)
{
    pemitter->tUnpause = D_002A514C;
}

/** @sly2-annot addr=0x15A160
 * Purpose: not written up yet; only the symbol name (pre-existing / Sly 1-derived) describes it.
 * Confidence: mid (draft) - evidence: pre-existing symbol name only; behaviour not reviewed
 * See: docs/reference/functions/particles-effects.md#emitter-0x159118-0x15cd68
 * @end-sly2-annot */
void UnpauseEmitter(EMITTER *pemitter)
{
    pemitter->tUnpause = -1.0f;
}

namespace M_FPausedEmitter__FP7EMITTER {
struct CLOCK { float t; float dt; };
extern "C" { extern CLOCK D_002DDED0; }
struct EMITTER { char pad[0xF0]; int k; int c; char pad2[0x148 - 0xF8]; float tUnpause; };
/** @sly2-annot addr=0x15A170
 * Purpose: not written up yet; only the symbol name (pre-existing / Sly 1-derived) describes it.
 * Confidence: mid (draft) - evidence: pre-existing symbol name only; behaviour not reviewed
 * See: docs/reference/functions/particles-effects.md#emitter-0x159118-0x15cd68
 * @end-sly2-annot */
extern "C" int FPausedEmitter__FP7EMITTER(EMITTER *pemitter)
{
    if (D_002DDED0.t < pemitter->tUnpause)
        return 1;
    switch (pemitter->k) {
    case 0:
    case 1:
        if (pemitter->c == 0)
            return 1;
    }
    return 0;
}
} // namespace M_FPausedEmitter__FP7EMITTER

/** @sly2-annot addr=0x15A1C0
 * Purpose: not written up yet; only the symbol name (pre-existing / Sly 1-derived) describes it.
 * Confidence: mid (draft) - evidence: pre-existing symbol name only; behaviour not reviewed
 * See: docs/reference/functions/particles-effects.md#emitter-0x159118-0x15cd68
 * @end-sly2-annot */
void OnEmitterValuesChanged(EMITTER *pemitter)
{
    pemitter->fDirty = 0;
    if (pemitter->pblipg != 0) {
        SetBlipgEmitb(pemitter->pblipg, pemitter->pemitb);
    }
    if (pemitter->pripg != 0) {
        SetRipgEmitb(pemitter->pripg, pemitter->pemitb);
    }
    if (pemitter->k == 3) {
        pemitter->tNext = GRandInRange(pemitter->lmRate.gMin, pemitter->lmRate.gMax) * 60.0f;
    } else {
        pemitter->tNext = GRandInRange(pemitter->lmRate.gMin, pemitter->lmRate.gMax);
    }
}

namespace M_SetEmitterParticleCount__FP7EMITTERi {
struct EMITTER { char pad[0xE0]; int fDirty; char pad2[0xC]; int k; int c; };
void PauseEmitterIndefinite(EMITTER *pemitter) __asm__("PauseEmitterIndefinite__FP7EMITTER");
void UnpauseEmitter(EMITTER *pemitter) __asm__("UnpauseEmitter__FP7EMITTER");
/** @sly2-annot addr=0x15A240
 * Purpose: not written up yet; only the symbol name (pre-existing / Sly 1-derived) describes it.
 * Confidence: mid (draft) - evidence: pre-existing symbol name only; behaviour not reviewed
 * See: docs/reference/functions/particles-effects.md#emitter-0x159118-0x15cd68
 * @end-sly2-annot */
extern "C" void SetEmitterParticleCount__FP7EMITTERi(EMITTER *pemitter, int c)
{
    switch (pemitter->k) {
    case 0:
        if (c == 0)
            PauseEmitterIndefinite(pemitter);
        else if (c == -1)
            UnpauseEmitter(pemitter);
        else
            pemitter->c = c;
        break;
    case -1:
        pemitter->c = c;
        break;
    }
    pemitter->fDirty = 1;
}
} // namespace M_SetEmitterParticleCount__FP7EMITTERi

/** @sly2-annot addr=0x15A2B0
 * Purpose: not written up yet; only the symbol name (pre-existing / Sly 1-derived) describes it.
 * Confidence: mid (draft) - evidence: pre-existing symbol name only; behaviour not reviewed
 * See: docs/reference/functions/particles-effects.md#emitter-0x159118-0x15cd68
 * @end-sly2-annot */
void SetEmitterAutoPause(EMITTER *pemitter, int fAutoPause)
{
    pemitter->fAutoPause = fAutoPause;
    pemitter->fDirty = 1;
}

namespace M_WakeSoWater__FP2SOP5WATERfP6VECTORT3ff {
// WakeSoWater (0x15A2C0): wake sound, ripple and splash for an SO moving through WATER.
// (Lilly's agent, T1683) MATCH, no register pins. Built from the lilly1/v3 drafts (Claude agents, T1679 / lane E20).
//
// Sly 2 signature (SO *, WATER *, qword pos, qword v, float dt, float gFrac, float gScale) despite the Sly 1 mangled name.
// The splash is func_0015BE28(pos, dir, cParticle, EMITP, EMITV, &e, 0, 0) with the EMITP (0x40) and EMITV (0x70) blocks
// passed BY VALUE (EABI: a3 / t0 point at the caller's copies). They are aggregate initializers: g++ builds each one
// in a memset stack temp and block-copies it, which is the target's memset + stores + lq/sq copy. What matched is the
// unions in the two types (as in func_0015CAD8): store_constructor emits a (clobber (mem:BLK)) for every union
// initializer, and those clobbers are scheduler barriers.
// - EMITV: G4V at +0x00, a 4-float union at +0x40 and Sly 1's CLQ at +0x60 give the target's FPU numbers for
//   5.0 / 0.39 / 1.047 / -1 (-1 reuses 0.39's register) and the store order.
// - EMITP: a CLQ at +0x18 ({8, 12, 1}; Sly 1's EMITRIP has a clqScale) puts the +0x10 store before the +0xC store.
//   Any union holding +0x18/+0x1C matches; field names other than the Sly 1 types are unknown.
// - E is 0x14 bytes (memset sizeof); the splash position (v raised 30 in z) is its own stack local.
typedef int qword __attribute__((mode(TI)));

// Sly 1 util.h CLQ: quadratic g0 + x*(g1 + x*g2).
union CLQ {
    struct { float g0, g1, g2, gUnused; };
    float ag[4];
};

// 16-byte float views; the field names are unknown (same type as in func_0015CAD8).
union G4V { struct { float g0, g4, g8, gC; }; float ag[4]; };

struct EMITP {
    int x0;
    int x4;
    int x8, xC, x10; // oids of WATER+0x178, WATER+0x17C and the WATER itself
    int x14;
    CLQ clq18;
    int x28, x2C;
    qword x30; // some 16-byte member: the target copies EMITP with lq/sq, so the type is 16-aligned
};

struct EMITV {
    G4V g0;
    qword v10;
    qword v20;
    int x30;
    int x34;
    float x38, x3C;
    G4V g40;
    int x50[4];
    CLQ clq60;
};

typedef char check_emitp[(sizeof(EMITP) == 0x40) ? 1 : -1];
typedef char check_emitv[(sizeof(EMITV) == 0x70) ? 1 : -1];

struct VTE { short d; short pad; qword (*pfn)(void *); };
struct VT { VTE a[64]; };
struct XE8 { char pad[0x80]; qword v; };
struct SO { int x0, x4, x8; VT *pvt; char pad10[0xE8 - 0x10]; XE8 *pxE8; };
struct SND { char pad[0x170]; float t; void *psnd; };
struct EVT { short d; short pad; void (*pfn)(void *, float, qword, int); };
struct EVTT { EVT a[4]; };
struct EX { char pad[0x18]; int id; };
struct WATER { char pad[0x18]; int x18; char pad1c[0x178 - 0x1C]; EX *px178; EX *px17C; int x180; };
struct RIP { char pad[0x1C]; EX *pex; float g20; float g24; char pad28[0xEC - 0x28]; EVTT *pvt; };
struct E { int x0, x4; EX *p8; EX *pC; WATER *p10; };
union V4 { qword q; float a[4]; };
struct CLOCK { float t; };

extern "C" {
extern CLOCK D_002DDED0;
extern int D_002BDEB0[];
extern qword D_002A2DC0;
extern char D_003DD490[];
float GRandInRange__Fff(float lo, float hi);
int NRandInRange__Fii(int lo, int hi);
int FIsBasicDerivedFrom__FP5BASIC3CID(SO *p, int n);
void func_001FB1F8(void *p, float g);
void StopSnd(void *p, float g);
void *PsndStartSound(int id);
void SetSndSourceLo(void *p, SO *pso);
void SetSndRange(void *p, float a, float b);
void RcPtr_Assign(void *pp, void *p);
RIP *PripNewRipg(void *p, int k, int x);
void *func_002333B4(void *p, int c, int cb);
void func_0015BE28(qword pos, qword dir, int n, EMITP emitp, EMITV emitv, E *pe, int x, int y);
}

static inline qword Length(qword a)
{
    qword r, one;
    __asm__("vmul.xyz %0, %2, %2\n\tvaddw.x %1, $vf0, $vf0w\n\tvadday.x $ACC, %0, %0y\n\tvmaddz.x %0, %1, %0z\n\t"
            "vsqrt $Q, %0x\n\tvwaitq\n\tvaddq.x %0, $vf0, $Q"
        : "=j"(r), "=j"(one) : "j"(a));
    return r;
}
static inline qword Sub(qword a, qword b)
{
    qword r;
    __asm__("vsub.xyzw %0, %1, %2" : "=j"(r) : "j"(a), "j"(b));
    return r;
}
static inline float GClamp(float g, float lo, float hi)
{
    if (g < lo)
        return lo;
    if (hi < g)
        return hi;
    return g;
}
static inline float GetX(qword q)
{
    float f;
    __asm__("mtc1 %1, %0" : "=f"(f) : "r"(q));
    return f;
}

void WakeSoWater(SO *p, WATER *pwater, qword pos, qword v, float dt, float gFrac, float gScale) __asm__("WakeSoWater__FP2SOP5WATERfP6VECTORT3ff");
/** @sly2-annot addr=0x15A2C0
 * Purpose: not written up yet; only the symbol name (pre-existing / Sly 1-derived) describes it.
 * Confidence: mid (draft) - evidence: pre-existing symbol name only; behaviour not reviewed
 * See: docs/reference/functions/particles-effects.md#emitter-0x159118-0x15cd68
 * @end-sly2-annot */
extern "C" void WakeSoWater__FP2SOP5WATERfP6VECTORT3ff(SO *p, WATER *pwater, qword pos, qword v, float dt, float gFrac, float gScale)
{
    float g = 0.0f;
    VTE *pvte = &p->pvt->a[33];
    float d = GetX(Length(Sub(pos, pvte->pfn((char *)p + pvte->d))));
    int fWake = GRandInRange__Fff(g, 3.0f) < dt;
    SND *psnd;
    float t, u;
    psnd = 0;
    if (FIsBasicDerivedFrom__FP5BASIC3CID(p, 0x32))
        psnd = (SND *)p;
    if (d > 10.0f || fWake) {
        u = (d - 10.0f) * 0.0012658228f;
        t = GClamp(u, g, 1.0f);
        if (psnd && pwater->x180) {
            void **pp = &psnd->psnd;
            g = t * 0.9f + 0.1f;
            if (psnd->psnd) {
                func_001FB1F8(psnd->psnd, g);
            } else {
                void *ps = PsndStartSound(D_002BDEB0[NRandInRange__Fii(0, 3)]);
                SetSndSourceLo(ps, (SO *)psnd);
                SetSndRange(ps, 100.0f, 1000.0f);
                func_001FB1F8(ps, g);
                RcPtr_Assign(pp, ps);
            }
        }
        if (!fWake) {
            u = 1.0f - t * (1.0f - gFrac);
            if (psnd) {
                fWake = u < D_002DDED0.t - psnd->t;
            } else {
                float tc = D_002DDED0.t;
                int c0 = (int)(tc / u);
                int c1 = (int)((tc + dt) / u);
                fWake = c0 != c1;
            }
        }
        if (fWake && pwater->px17C) {
            RIP *prip = PripNewRipg(D_003DD490, 7, 0);
            if (prip) {
                EVT *pe = &prip->pvt->a[1];
                pe->pfn((char *)prip + pe->d, gScale, v, 0);
                prip->g20 = gScale;
                prip->g24 = gScale * 0.6f;
                prip->pex = pwater->px17C;
                if (psnd)
                    psnd->t = D_002DDED0.t;
                if (500.0f < d && pwater->px178) {
                    EMITP emitp = { 1, 0, pwater->px178->id, pwater->px17C->id, pwater->x18, 0, {{ 8.0f, 12.0f, 1.0f }} };
                    EMITV emitv = { {{ d * 0.5f, d * 0.75f, 0.0f, 1.0f }}, p->pxE8->v, D_002A2DC0, 0, 0, 1.0f, 5.0f,
                                    {{ 0.39269909f, 1.0471976f }}, { 0 }, {{ 1.0f, 0.0f, -1.0f }} };
                    E e;
                    V4 posSplash;
                    func_002333B4(&e, 0, sizeof(e));
                    posSplash.q = v;
                    posSplash.a[2] += 30.0f;
                    e.p8 = pwater->px178;
                    e.pC = pwater->px17C;
                    e.p10 = pwater;
                    func_0015BE28(posSplash.q, D_002A2DC0, NRandInRange__Fii(4, 6), emitp, emitv, &e, 0, 0);
                }
            }
        }
    } else if (psnd) {
        void **pp = &psnd->psnd;
        if (pp && *pp) {
            StopSnd(*pp, g);
            RcPtr_Assign(pp, 0);
        }
    }
}
} // namespace M_WakeSoWater__FP2SOP5WATERfP6VECTORT3ff

namespace M_ChooseEmitoPos__FP5EMITOiiP6VECTORT3 {
// ChooseEmitoPos: EMITB choose emit position/direction (Claude agent, lanes E3-E11).
// Notes: the unused case-1 VU_VECTOR[3] frees the stack slot that case 3's an[] reuses; VLen = one length-squared asm
// plus a separate vsqrt asm; Normalize returns its VU_VECTOR by value (gives dir its stack slot).
typedef int qword __attribute__((mode(TI)));
struct VU_FLOAT { qword q; VU_FLOAT(float f) { __asm__("qmtc2.ni %1, %0" : "=j"(q) : "r"(f)); } };
struct VU_VECTOR { qword q; };
static inline VU_VECTOR V(qword q) { VU_VECTOR v; v.q = q; return v; }
struct MATRIX3 { qword r[3]; };
struct CM { char pad[0x60]; MATRIX3 mat; };
extern "C" { extern CM D_002DDF40; }
extern "C" { extern qword D_002A2DA0; }
struct TRI { int aipos[3]; float sArea; };
struct EMITMESH { int cpos; qword *apos; int ctri; TRI *atri; qword *anormal; float sArea; };
struct BOXE { float xMin, yMin, zMin, pad; float xMax, yMax, zMax; };
struct MESHO { char pad[0xE8]; int cs; int padEC; float *as; };
struct EMITO { void *palo; MESHO *pmesh; };
struct LM { float gMin, gMax; };
struct EP {
    int kind;
    int kindDir;
    int pad18[3];
    LM lm;
    int pad2C;
    void *pv30;
    int pad34[3];
    qword v40;
};
struct EMITB {
    int pad0, pad4;
    EMITO *pemito;
    int padC;
    struct EP ep;
};
extern "C" {
float GRandInRange__Fff(float gMin, float gMax);
void EvaluateShapeS(MESHO *pmesh, float g, qword *ppos, qword *pnormal);
MATRIX3 *func_001A6318(void *palo);
}
static inline qword Sub(qword a, qword b) { qword d; __asm__("vsub.xyzw %0, %1, %2" : "=j"(d) : "j"(a), "j"(b)); return d; }
static inline qword Add(qword a, qword b) { qword d; __asm__("vadd.xyzw %0, %1, %2" : "=j"(d) : "j"(a), "j"(b)); return d; }
static inline qword Cross(qword a, qword b)
{
    qword d;
    __asm__("vopmula.xyz $ACC, %1, %2\n\tvopmsub.xyz %0, %2, %1" : "=j"(d) : "j"(a), "j"(b));
    return d;
}
static inline qword Combo3(VU_FLOAT f0, VU_VECTOR v0, VU_FLOAT f1, VU_VECTOR v1, VU_FLOAT f2, VU_VECTOR v2)
{
    qword d;
    __asm__("vmulax.xyzw $ACC, %2, %1x\n\tvmaddax.xyzw $ACC, %4, %3x\n\tvmaddx.xyzw %0, %6, %5x"
            : "=j"(d) : "j"(f0.q), "j"(v0.q), "j"(f1.q), "j"(v1.q), "j"(f2.q), "j"(v2.q));
    return d;
}
static inline qword MulMat(const MATRIX3 &m, qword v)
{
    qword d;
    __asm__("vmulax.xyz $ACC, %2, %1x\n\tvmadday.xyz $ACC, %3, %1y\n\tvmaddz.xyz %0, %4, %1z"
            : "=j"(d) : "j"(v), "j"(m.r[0]), "j"(m.r[1]), "j"(m.r[2]));
    return d;
}
static inline qword MulTMat(const MATRIX3 &m, qword v)
{
    qword d, one, t0, t1, t2;
    __asm__ __volatile__("vmul.xyzw %2, %5, %8\n\tvmaxw.xyzw %1, $vf0, $vf0w\n\tvmul.xyzw %3, %6, %8\n\tvmul.xyzw %4, %7, %8\n\t"
            "vadday.x $ACC, %2, %2y\n\tvmaddz.x %0, %1, %2z\n\tvaddax.y $ACC, %3, %3x\n\tvmaddz.y %0, %1, %3z\n\tvaddax.z $ACC, %4, %4x\n\tvmaddy.z %0, %1, %4y"
            : "=j"(d), "=&j"(one), "=&j"(t0), "=&j"(t1), "=&j"(t2) : "j"(m.r[0]), "j"(m.r[1]), "j"(m.r[2]), "j"(v));
    return d;
}
static inline qword MulCam(CM *pcm, qword v) { return MulMat(pcm->mat, v); }
static __inline__ qword VLen(qword c)
{
    qword l, one;
    __asm__("vmul.xyz %1, %2, %2\n\tvaddw.x %0, $vf0, $vf0w\n\tvadday.x $ACC, %1, %1y\n\tvmaddz.x %1, %0, %1z" : "=j"(one), "=&j"(l) : "j"(c) : "memory");
    {
    qword r;
    __asm__("vsqrt $Q, %1x\n\tvwaitq\n\tvaddq.x %0, $vf0, $Q" : "=j"(r) : "j"(l));
    return r;
    }
}
static inline qword Scale(VU_FLOAT f, VU_VECTOR v) { qword r; __asm__("vmulx.xyzw %0, %2, %1x" : "=j"(r) : "j"(f.q), "j"(v.q)); return r; }

static inline VU_VECTOR Normalize(qword d)
{
    VU_VECTOR dir;
    qword l = VLen(d);
    qword k = D_002A2DA0;
    float gl;
    __asm__("qmfc2.ni %0, %1" : "=r"(gl) : "j"(l));
    if (gl < 1e-4f) {
        dir.q = k;
    } else {
        qword dq;
        __asm__ __volatile__("nop\n\tvdiv $Q, $vf0w, %2x\n\tvwaitq\n\tvmulq.xyzw %0, %1, $Q" : "=j"(dq) : "j"(d), "j"(l));
        dir.q = dq;
    }
    return dir;
}
/** @sly2-annot addr=0x15A808
 * Purpose: Chooses a particle origin position/direction from the EMITO shape (point/sphere/box/skeleton)
 * Info: proposed name ChooseEmitoPos (T1); TU emitter
 * Notes: verifier: purpose partly wrong: Sly 1 ChooseEmitoPos(emito,i,c,ppos,pnormal). Cases are
 *   point(0)/box(1)/curve(2, cumulative length)/mesh(3, area-weighted triangle), not 'sphere/skeleton'.
 * Confidence: high [T1 0.85] - evidence: Sly 1 / Lilly reference; static naming pass; sources:
 *   r3_1539A8+verdicts_tu_r3r5+verdicts_A1+names_v2-signature(verified)
 * See: docs/engine/particles-effects.md#6-key-functions,
 *   docs/reference/functions/particles-effects.md#emitter-0x159118-0x15cd68
 * @end-sly2-annot */
extern "C" void ChooseEmitoPos__FP5EMITOiiP6VECTORT3(EMITB *p, int i, int c, qword *ppos, qword *pv)
{
    EP *pep = &p->ep;
    LM *plm;
    {
    VU_VECTOR av[3];
    switch (p->ep.kind) {
    case 0:
        *ppos = *(qword *)&pep->pv30;
        break;
    case 1: {
        { VU_VECTOR avUnused[3]; } // unused 48-byte local: frees the slot an[] reuses in case 3
        BOXE *pbox = (BOXE *)pep->pv30;
        float *pf = (float *)ppos;
        pf[0] = GRandInRange__Fff(pbox->xMin, pbox->xMax);
        pf[1] = GRandInRange__Fff(pbox->yMin, pbox->yMax);
        pf[2] = GRandInRange__Fff(pbox->zMin, pbox->zMax);
        break;
    }
    case 2: {
        MESHO *pmesh = p->pemito->pmesh;
        if (!pmesh) {
            float *pf = (float *)ppos;
            pf[2] = 0.0f;
            pf[1] = 0.0f;
            pf[0] = 0.0f;
            return;
        }
        float ds = pmesh->as[pmesh->cs - 1] / (float)c;
        float s = (float)i * ds;
        float g = GRandInRange__Fff(s, s + ds);
        switch (pep->kindDir) {
        default:
            EvaluateShapeS(pmesh, g, ppos, 0);
            break;
        case 2:
            EvaluateShapeS(pmesh, g, ppos, pv);
            goto normalize;
        case 3: {
            EvaluateShapeS(pmesh, g, ppos, &av[0].q);
            *pv = Cross(av[0].q, p->ep.v40);
            goto normalize;
        }
        }
        break;
    }
    case 3: {
        EMITMESH *pem = (EMITMESH *)pep->pv30;
        float s = GRandInRange__Fff(0.0f, 1.0f) * pem->sArea;
        TRI *ptri;
        int itri;
        for (itri = 0; itri < pem->ctri; itri++) {
            ptri = &pem->atri[itri];
            s -= ptri->sArea;
            if (s <= 0.0f)
                break;
        }
        av[0].q = pem->apos[ptri->aipos[0]];
        av[1].q = pem->apos[ptri->aipos[1]];
        av[2].q = pem->apos[ptri->aipos[2]];
        float u = GRandInRange__Fff(0.0f, 1.0f);
        float v = GRandInRange__Fff(0.0f, 1.0f - u);
        float w = 1.0f - u - v;
        *ppos = Combo3(u, av[0], v, av[1], w, av[2]);
        if (pep->kindDir == 5) {
            VU_VECTOR an[3];
            an[0].q = pem->anormal[ptri->aipos[0]];
            an[1].q = pem->anormal[ptri->aipos[1]];
            an[2].q = pem->anormal[ptri->aipos[2]];
            *pv = Combo3(u, an[0], v, an[1], w, an[2]);
            goto normalize;
        }
        break;
    }
    }

    switch (pep->kindDir) {
    case 0:
        *pv = pep->v40;
        break;
    case 1:
        *pv = Sub(*ppos, pep->v40);
        break;
    case 4: {
        void *palo = p->pemito->palo;
        if (palo) {
            MATRIX3 *pm = func_001A6318(palo);
            *pv = MulTMat(*pm, MulCam(&D_002DDF40, pep->v40));
        } else {
            *pv = MulCam(&D_002DDF40, pep->v40);
        }
        break;
    }
    default:
        *pv = D_002A2DA0;
        break;
    }
    }

normalize:
    plm = &p->ep.lm;
    {
        VU_VECTOR dir = Normalize(*pv);
        *pv = dir.q;
        qword pos = *ppos;
        *ppos = Add(pos, Scale(VU_FLOAT(GRandInRange__Fff(pep->lm.gMin, plm->gMax)), dir));
    }
}
} // namespace M_ChooseEmitoPos__FP5EMITOiiP6VECTORT3

namespace M_ConvertEmitoPosVec__FP5EMITOP6VECTORT1 { extern "C" {
typedef int qword __attribute__((mode(TI)));
struct M4 { qword r[4]; };
struct R { char pad[0x40]; struct M4 m; };
struct BUF { qword a[4]; };
struct VTE { short d; short pad; struct R *(*pfn)(void *, struct BUF *); };
struct SO { char pad[0xC]; struct VTE *vt; };
struct Y { int x0, x4, k; };
struct E { int x0, x4; struct SO **ppso; int xC; struct Y y; };
void __ml__FRC7MATRIX4T0(struct M4 *pm, struct M4 *pa, struct BUF *pb);
qword func_001A72D0(struct M4 *pm, int z, qword v);
qword func_001A7348(struct M4 *pm, int z, qword v);
qword GetXfVelocity(struct R *r, int z, qword v);
static inline qword Add(qword a, qword b) { qword d; __asm__("vadd.xyzw %0, %1, %2" : "=j"(d) : "j"(a), "j"(b)); return d; }

/** @sly2-annot addr=0x15AD48
 * Purpose: Transforms a chosen particle position (and velocity, depending on mode) from the reference SO
 *   space to world space
 * Info: proposed name ConvertEmitoPosVec (T1); TU emitter
 * Confidence: mid (draft) [T1 0.70] - evidence: static naming pass; sources:
 *   r3_1539A8+verdicts_A1+names_v2-signature(verified)
 * See: docs/engine/particles-effects.md#6-key-functions,
 *   docs/reference/functions/particles-effects.md#emitter-0x159118-0x15cd68
 * @end-sly2-annot */
void ConvertEmitoPosVec__FP5EMITOP6VECTORT1(struct E *p, qword *pv, qword *pw)
{
    struct Y *py = &p->y;
    struct SO *pso = *p->ppso;
    struct BUF buf;
    struct M4 m;
    struct R *r;
    if (!pso) return;
    r = pso->vt[32].pfn((char *)pso + pso->vt[32].d, &buf);
    __ml__FRC7MATRIX4T0(&m, &r->m, &buf);
    *pv = func_001A72D0(&m, 0, *pv);
    if (py->k != 3) {
        *pw = func_001A7348(&m, 0, *pw);
        if (py->k != 2)
            *pw = Add(*pw, GetXfVelocity(r, 0, py->k == 1 ? *pv : r->m.r[3]));
    }
}
} } // namespace M_ConvertEmitoPosVec__FP5EMITOP6VECTORT1

namespace M_CalculateEmitvx__FiP2LMiP6EMITVX {
struct LM { float gMin; float gMax; };
struct EMITVX { int c; float dg; float drad; float g; float rad; };
float RadNormalize(float rad) asm("RadNormalize__Ff");
float GRandInRange(float gMin, float gMax) asm("GRandInRange__Fff");
/** @sly2-annot addr=0x15AE38
 * Purpose: not written up yet; only the symbol name (pre-existing / Sly 1-derived) describes it.
 * Confidence: mid (draft) - evidence: pre-existing symbol name only; behaviour not reviewed
 * See: docs/reference/functions/particles-effects.md#emitter-0x159118-0x15cd68
 * @end-sly2-annot */
extern "C" void CalculateEmitvx__FiP2LMiP6EMITVX(int c, LM *plm, int cMax, EMITVX *pemitvx)
{
    int n;
    float dg;
    if (c > 0)
        pemitvx->c = c < cMax ? c : cMax;
    else
        pemitvx->c = cMax;
    n = (cMax + (pemitvx->c - 1)) / pemitvx->c;
    dg = RadNormalize(plm->gMax - plm->gMin);
    pemitvx->drad = 6.2831855f / pemitvx->c;
    pemitvx->dg = dg / n;
    pemitvx->g = plm->gMin + pemitvx->dg * 0.5f;
    pemitvx->rad = GRandInRange(0.0f, 6.2831855f);
}
} // namespace M_CalculateEmitvx__FiP2LMiP6EMITVX

namespace M_ChooseEmitVelocity__FP6EMITVXffP2LMP6VECTORiT4 { extern "C" {
typedef int qword __attribute__((mode(TI)));
typedef union { qword q; float f[4]; } U;
typedef struct { qword r[3]; } M3;
struct EMITVX { int c; float dTilt; float dPan; float tiltBase; float panBase; };
struct LM { float gMin; float gMax; };
extern qword D_002A2DA0;
extern qword D_002A2DB0;
float GRandInRange__Fff(float a, float b);
float RadNormalize__Ff(float g);
qword SetVectorSphere(float pan, float tilt, float r);
void GetNormalVectors(qword a, qword b, qword c, M3 *pm);

/** @sly2-annot addr=0x15AF08
 * Purpose: not written up yet; only the symbol name (pre-existing / Sly 1-derived) describes it.
 * Confidence: mid (draft) - evidence: pre-existing symbol name only; behaviour not reviewed
 * See: docs/reference/functions/particles-effects.md#emitter-0x159118-0x15cd68
 * @end-sly2-annot */
void ChooseEmitVelocity__FP6EMITVXffP2LMP6VECTORiT4(struct EMITVX *pemitvx, float gRand, float sv, struct LM *plm,
                                                     qword *pnormal, int i, qword *pv)
{
    int c = pemitvx->c;
    int iTilt = i / c;
    int iPan = i % c;
    float tilt = pemitvx->tiltBase + (float)iTilt * pemitvx->dTilt;
    float pan = pemitvx->panBase + (float)iPan * pemitvx->dPan;
    U v;
    M3 m;
    qword r;
    if (gRand != 0.0f) {
        tilt += gRand * GRandInRange__Fff(-0.5f, 0.5f) * pemitvx->dTilt;
        pan += gRand * GRandInRange__Fff(-0.5f, 0.5f) * pemitvx->dPan;
    }
    tilt = RadNormalize__Ff(tilt + 1.5707964f);
    pan = RadNormalize__Ff(pan);
    v.q = SetVectorSphere(pan, tilt, GRandInRange__Fff(plm->gMin, plm->gMax));
    v.f[2] *= sv;
    m.r[2] = *pnormal;
    GetNormalVectors(*pnormal, D_002A2DA0, D_002A2DB0, &m);
    __asm__("vmulax.xyz $ACC, %2, %1x\n\tvmadday.xyz $ACC, %3, %1y\n\tvmaddz.xyz %0, %4, %1z"
            : "=j"(r) : "j"(v.q), "j"(m.r[0]), "j"(m.r[1]), "j"(m.r[2]));
    *pv = r;
}
} } // namespace M_ChooseEmitVelocity__FP6EMITVXffP2LMP6VECTORiT4

namespace M_ChooseEmitvVelocityAge__FP5EMITVP6EMITVXP5EMITOiP6VECTORN24PfT7 { extern "C" {
typedef int qword __attribute__((mode(TI)));
typedef union { qword q; float f[4]; } U;
struct P { char pad[0xC]; float gC; char pad1[0x38 - 0x10]; float g38; float g3C; char pad2[0x48 - 0x40]; float g48; float g4C; float g50; float g54; };
struct CLOCK { float t; };
extern struct CLOCK D_002DDED0;
void ChooseEmitVelocity__FP6EMITVXffP2LMP6VECTORiT4(void *pemitvx, float a, float b, struct P *plm, qword *pv, int n, qword *pvOut);
float GRandInRange__Fff(float a, float b);

/** @sly2-annot addr=0x15B0C8
 * Purpose: Chooses per-particle velocity (ChooseEmitVelocity) and age
 * Info: proposed name ChooseEmitvVelocityAge (T1); TU emitter
 * Confidence: mid (draft) [T1 0.65] - evidence: static naming pass; sources:
 *   r3_1539A8+verdicts_A1+names_v2-signature(verified)
 * See: docs/engine/particles-effects.md#6-key-functions,
 *   docs/reference/functions/particles-effects.md#emitter-0x159118-0x15cd68
 * @end-sly2-annot */
void ChooseEmitvVelocityAge__FP5EMITVP6EMITVXP5EMITOiP6VECTORN24PfT7(struct P *p, void *pemitvx, int unused, int n, qword *ppos, qword *pv, qword *pvel, float *ptBirth, float *ptDeath)
{
    float t, g;
    ChooseEmitVelocity__FP6EMITVXffP2LMP6VECTORiT4(pemitvx, p->g38, p->gC, p, pv, n, pvel);
    t = D_002DDED0.t;
    t = t - GRandInRange__Fff(p->g50, p->g54);
    *ptBirth = t;
    *ptDeath = t + p->g3C;
    g = GRandInRange__Fff(p->g48, p->g4C);
    if (g != 0.0f) {
        U u;
        qword r;
        {
            qword qwuOut;
            __asm__("qmtc2.ni %1, %0" : "=j"(qwuOut) : "r"(g));
            u.q = qwuOut;
        }
        __asm__("vadda.xyzw $ACC, %1, $vf0\n\tvmaddx.xyzw %0, %2, %3x" : "=j"(r) : "j"(*ppos), "j"(*pvel), "j"(u.q));
        *ppos = r;
    }
}
} } // namespace M_ChooseEmitvVelocityAge__FP5EMITVP6EMITVXP5EMITOiP6VECTORN24PfT7

namespace M_EmitRips__FP5EMITBP5EMITGiP6VECTORT3PfT5 {
typedef int qword __attribute__((mode(TI)));
struct MATRIX3 { float mat[3][4]; } __attribute__((aligned(16)));
struct EMITTER;
struct SHD { void *p0; int x4; int x8; int pad; int x10; };
struct EMITTER {
    char pad0[8];
    SHD *pshd;
    char pad1[0xB0 - 0xC];
    qword qB0;
    char pad2[0xC0 - 0xC0];
    int xC0;
    int xC4;
    char pad3[0xD8 - 0xC8];
    float gD8, gDC;
    char pad4[4];
    float gE4, gE8;
};
class PART {
public:
    char pad0[0x14];
    float x14, x18;
    int x1C;
    char pad20[4];
    float x24, x28;
    char pad2C[4];
    qword x30;
    MATRIX3 mat40;
    char pad70[0x20];
    qword x90;
    char abA0[0xEC - 0xA0];
    virtual void Set(qword pos, float g, int n);
    virtual void Init(EMITTER *pemit);
};
class SO {
public:
    char pad[0xC];
    virtual void V1();
    virtual void V2();
    virtual void V3();
    virtual void V4();
    virtual void V5();
    virtual void V6();
    virtual void V7();
    virtual void V8();
    virtual void V9();
};
struct EMITX { int x0; SO **ppso; void *p8; };
extern "C" {
extern char D_003DD490[];
SO *PripgNew(void *p, int n);
void SetRipgEmitb__FP4RIPGP5EMITB(SO *pso, EMITTER *pemit);
void SubscribeLoObject(SO *pso, void *p);
PART *PripNewRipg(void *p, int n, SO *pso);
float GRandInRange__Fff(float a, float b);
void SetEmitdvEmitb__FP6EMITDVP5EMITB(void *p, EMITTER *pemit);
MATRIX3 *func_001A6318(void *p);
int FIsDlEmpty__FP2DL(void *p);
}
/** @sly2-annot addr=0x15B1B0
 * Purpose: Spawns RIP particles (PripNewRipg, SetRipgEmitb, SetEmitdvEmitb) for originated positions
 * Info: proposed name EmitRips (T1); TU emitter
 * Confidence: high [T1 0.85] - evidence: static naming pass; sources:
 *   r3_1539A8+verdicts_tu_r3r5+verdicts_A1+names_v2-signature(verified)
 * See: docs/engine/particles-effects.md#6-key-functions,
 *   docs/reference/functions/particles-effects.md#emitter-0x159118-0x15cd68
 * @end-sly2-annot */
extern "C" void EmitRips__FP5EMITBP5EMITGiP6VECTORT3PfT5(EMITTER *pemit, EMITX *px, int c, qword *apos, qword *avel, float *at0, float *at1)
{
    SO *pso = 0;
    int i;
    PART *ppart;
    if (px && px->ppso)
        pso = *px->ppso;
    if (!pso) {
        pso = PripgNew(D_003DD490, pemit->xC4);
        if (!pso)
            return;
        if (px && px->ppso)
            *px->ppso = pso;
        SetRipgEmitb__FP4RIPGP5EMITB(pso, pemit);
        if (px && px->p8)
            SubscribeLoObject(pso, px->p8);
    }
    if (!pso)
        return;
    for (i = 0; i < c && (ppart = PripNewRipg(D_003DD490, pemit->xC0, pso)) != 0; i++) {
        ppart->Set(apos[i], GRandInRange__Fff(pemit->gD8, pemit->gDC), pemit->pshd->x10);
        if (pemit->gE4 != 0.0f || pemit->gE8 != 0.0f) {
            ppart->x24 = pemit->gE4;
            ppart->x28 = pemit->gE8;
        }
        ppart->x90 = avel[i];
        ppart->x14 = at0[i];
        ppart->x18 = at1[i] - at0[i];
        ppart->x30 = pemit->qB0;
        SetEmitdvEmitb__FP6EMITDVP5EMITB(ppart->abA0, pemit);
        ppart->x1C = pemit->pshd->x8;
        if (pemit->pshd->p0)
            ppart->mat40 = *func_001A6318(pemit->pshd->p0);
        ppart->Init(pemit);
    }
    if (FIsDlEmpty__FP2DL((char *)pso + 0x15C))
        pso->V9();
}
} // namespace M_EmitRips__FP5EMITBP5EMITGiP6VECTORT3PfT5

namespace M_EmitBlips__FP5EMITBP5EMITGiP6VECTORT3PfT5T3T3 {
// MATCH (Claude agent, lane E21), from fresh3 (56). Fixes: (1) for-increment written `k++, i++` (loop.c prepends each biv to loop_iv_list, so the LAST-incremented biv's giv inits are emitted first in the preheader; the target computes i*16 before k*16/k*4, which clobbers v1 and stops reload inheriting k from the k<c test); (2) Dot's d output `=j` (not earlyclobber) so it shares a's vf2.
// DRAFT (Claude agent, lane E5): fresh rewrite, aligned 49 (old 80). Outer while(k<c && (pblk || (pblk=alloc))) gives the target loop shape; GRand(&p->lm) inline gives the hoisted plm slot; unsigned n0 &= ~1u gives lui/ori. Left: giv bases (target base = &apd[i] for a-side, separate &afr[1].apos[i] giv for the copy).
typedef int qword __attribute__((mode(TI)));
struct EMITTER;
struct PD { unsigned int n0; int n4; float g8; float gC; };
struct PAIR { qword a; qword b; };
struct FR { qword apos[43]; qword av[43]; qword apd[43]; qword pad[3]; };
struct BLK {
    FR afr[2]; int x1080;
    int c; PAIR *apair; int pad108C; BLK *pblkNext;
};
struct PRG;
struct PRGVT { char pad[0x198]; short d; short pad2; void (*pfn)(PRG *, EMITTER *); };
struct PRG {
    char pad0[0xC]; PRGVT *pvt; char pad10[0x160 - 0x10]; int k160; char pad164[0x1A0 - 0x164]; int c1A0;
    char pad1A4[0x3B0 - 0x1A4]; int c3B0; char pad3B4[0x434 - 0x3B4]; int f434; int pad438; int k43C; int pad440;
    int c444; BLK *pblkFirst;
};
struct LM { float gMin, gMax; };
struct EMITTER { char pad[0xC8]; int fC8; char padCC[0xF0 - 0xCC]; int kF0; int fF4; LM lm; };
struct EMITX { PRG **ppprg; void *ppripg; EMITTER *pemitter; };
struct CM { char pad[0x70]; qword dir; };
extern "C" {
extern CM D_002DDF40;
extern char D_003DD490[];
PRG *PblipgNew(void *pv);
void SetBlipgEmitb(PRG *pprg, EMITTER *p);
BLK *PblipNew(void *pv, PRG *pprg);
void *func_002333B4(void *pv, int b, int cb);
int NRandInRange__Fii(int nMin, int nMax);
float GRandInRange__Fff(float gMin, float gMax);
void EmitBlips__FP5EMITBP5EMITGiP6VECTORT3PfT5T3T3(EMITTER *p, EMITX *px, int c, qword *apos, qword *av, float *ag1, float *ag2, qword *aa, qword *ab);
}

static inline float Dot(qword a, qword b)
{
    qword d, t;
    float g;
    __asm__("vmul.xyz %0, %2, %3\n\tvaddw.x %1, $vf0, $vf0w\n\tvadday.x $ACC, %0, %0y\n\tvmaddz.x %0, %1, %0z" : "=j"(d), "=&j"(t) : "j"(a), "j"(b));
    __asm__("qmfc2.ni %0, %1" : "=r"(g) : "j"(d));
    return g;
}

static inline float GRand(LM *plm) { return GRandInRange__Fff(plm->gMin, plm->gMax); }

/** @sly2-annot addr=0x15B428
 * Purpose: Spawns BLIP particles for originated positions
 * Info: proposed name EmitBlips (T1); TU emitter
 * Confidence: mid (draft) [T1 0.65] - evidence: static naming pass; sources:
 *   r3_1539A8+verdicts_A1+names_v2-signature(verified)
 * See: docs/engine/particles-effects.md#6-key-functions,
 *   docs/reference/functions/particles-effects.md#emitter-0x159118-0x15cd68
 * @end-sly2-annot */
void EmitBlips__FP5EMITBP5EMITGiP6VECTORT3PfT5T3T3(EMITTER *p, EMITX *px, int c, qword *apos, qword *av, float *ag1, float *ag2, qword *aa, qword *ab)
{
    PRG *pprg = 0;
    if (px && px->ppprg)
        pprg = *px->ppprg;
    if (!pprg) {
        pprg = PblipgNew(D_003DD490);
        if (!pprg)
            return;
        if (px && px->ppprg)
            *px->ppprg = pprg;
        SetBlipgEmitb(pprg, p);
        if (px && px->pemitter)
            pprg->pvt->pfn((PRG *)((char *)pprg + pprg->pvt->d), px->pemitter);
        if (!pprg)
            return;
    }
    BLK *pblk = pprg->pblkFirst;
    int k = 0;
    while (k < c && (pblk || (pblk = PblipNew(D_003DD490, pprg)))) {
        qword *pq1 = pblk->afr[1].apos;
        for (unsigned int i = pblk->c; i < 43 && k < c; k++, i++) {
            FR *pfr = &pblk->afr[0];
            pfr->apos[i] = apos[k];
            ((float *)&pfr->apos[i])[3] = ag1[k];
            pfr->av[i] = av[k];
            ((float *)&pfr->av[i])[3] = ag2[k];
#define pd ((PD *)&pfr->apd[i])
            func_002333B4(pd, 0, sizeof(PD));
            pd->n0 = NRandInRange__Fii(0, pprg->c3B0 - 1);
            if (pprg->f434)
                pd->n0 &= ~1u;
            if (p->fC8)
                pd->n4 = NRandInRange__Fii(0, pprg->c1A0 - 1);
            if (pprg->k43C == 2) {
                float g = GRand(&p->lm);
                pd->g8 = g;
                if (!p->kF0 && 0.0f < Dot(D_002DDF40.dir, pfr->av[i]))
                    pd->g8 = -g;
                if (p->fF4)
                    pd->gC = GRandInRange__Fff(-3.14159265f, 3.14159265f);
            }
            if (pprg->k160 == 2) {
                PAIR *ppair = &pblk->apair[i];
                ppair->a = aa[k];
                ppair->b = ab[k];
            }
            pq1[i] = pfr->apos[i];
            pq1[i + 43] = pfr->av[i];
            pq1[i + 86] = *(qword *)pd;
            pblk->c++;
            pprg->c444++;
        }
        pblk = pblk->pblkNext;
    }
}
} // namespace M_EmitBlips__FP5EMITBP5EMITGiP6VECTORT3PfT5T3T3

namespace M_OriginateParticles__FiP5EMITBP7EMITGEN {
// (Claude agent, lane E7) MATCH from the 4-diff wip: ONE register pin, the pb->apos base in $2 (v0) inside the pxf block.
// NEAR MISS (round 9): 4/130, only $v0/$v1 for &apos[i] in the first pxf block
typedef int qword __attribute__((mode(TI)));
struct AL { int cb; int cbAlign; AL(int c, int a) { cb = c; cbAlign = a; } };
struct MATRIX3 { qword r[3]; };
struct XF { qword pos; MATRIX3 mat; qword v; qword w; };
struct EMITVX { int c; float dTilt; float dPan; float tiltBase; float panBase; };
struct EMITTER { char pad[0x1C]; XF *pxf; char pad1[0x84 - 0x20]; int x84; char pad2[0x90 - 0x88]; char x90[1]; };
struct BUF { qword *apos; qword *av; float *ag; float *an; int f10; };
extern "C" {
void *func_001AE318(AL *pal);
void CalculateEmitvx__FiP2LMiP6EMITVX(int n, void *p, int c, EMITVX *pvx);
void ChooseEmitoPos__FP5EMITOiiP6VECTORT3(EMITTER *pemit, int i, int c, qword *ppos, qword *pnormal);
void ChooseEmitvVelocityAge__FP5EMITVP6EMITVXP5EMITOiP6VECTORN24PfT7(void *p, EMITVX *pvx, void *q, int n, qword *ppos, qword *pnormal, qword *pvel, float *ptBirth, float *ptDeath);
void ConvertEmitoPosVec__FP5EMITOP6VECTORT1(EMITTER *pemit, qword *ppos, qword *pvel);
void OriginateParticles__FiP5EMITBP7EMITGEN(int c, EMITTER *pemit, BUF *pb);
}
static inline void *PvAlloc(int cb, int cbAlign) { AL al(cb, cbAlign); return func_001AE318(&al); }
static inline qword MulRow(const MATRIX3 &m, qword v)
{
    qword r;
    __asm__("vmulax.xyz ACC, %2, %1x\n\tvmadday.xyz ACC, %3, %1y\n\tvmaddz.xyz %0, %4, %1z" : "=j"(r) : "j"(v), "j"(m.r[0]), "j"(m.r[1]), "j"(m.r[2]));
    return r;
}
static inline qword Add(qword a, qword b) { qword d; __asm__("vadd.xyzw %0, %1, %2" : "=j"(d) : "j"(a), "j"(b)); return d; }
static inline qword Sub(qword a, qword b) { qword d; __asm__("vsub.xyzw %0, %1, %2" : "=j"(d) : "j"(a), "j"(b)); return d; }
static inline qword Cross(qword a, qword b)
{
    qword d;
    __asm__("vopmula.xyz ACC, %1, %2\n\tvopmsub.xyz %0, %2, %1" : "=j"(d) : "j"(a), "j"(b));
    return d;
}
/** @sly2-annot addr=0x15B7E8
 * Purpose: Computes origin position/velocity/age for c particles (CalculateEmitvx, ChooseEmitoPos,
 *   ChooseEmitvVelocityAge, ConvertEmitoPosVec)
 * Info: proposed name OriginateParticles (T1); TU emitter
 * Confidence: mid (draft) [T1 0.70] - evidence: static naming pass; sources:
 *   r3_1539A8+verdicts_A1+names_v2-signature(verified)
 * See: docs/engine/particles-effects.md#6-key-functions,
 *   docs/reference/functions/particles-effects.md#emitter-0x159118-0x15cd68
 * @end-sly2-annot */
void OriginateParticles__FiP5EMITBP7EMITGEN(int c, EMITTER *pemit, BUF *pb)
{
    pb->apos = (qword *)PvAlloc(c * 16, 16);
    pb->av = (qword *)PvAlloc(c * 16, 16);
    pb->ag = (float *)PvAlloc(c * 4, 16);
    pb->an = (float *)PvAlloc(c * 4, 16);
    EMITVX vx;
    CalculateEmitvx__FiP2LMiP6EMITVX(pemit->x84, pemit->x90, c, &vx);
    for (int i = 0; i < c; i++) {
        union { qword q; float f[4]; } normal;
        ChooseEmitoPos__FP5EMITOiiP6VECTORT3(pemit, i, c, &pb->apos[i], &normal.q);
        ChooseEmitvVelocityAge__FP5EMITVP6EMITVXP5EMITOiP6VECTORN24PfT7((char *)pemit + 0x50, &vx, (char *)pemit + 0x10, i, &pb->apos[i], &normal.q, &pb->av[i], &pb->ag[i], &pb->an[i]);
        XF *pxf = pemit->pxf;
        if (pxf) {
            {
                register qword *ppos __asm__("$2"); // register pin (target loads pb->apos into v0 and forms &apos[i] there)
                ppos = pb->apos;
                ppos[i] = Add(MulRow(pxf->mat, ppos[i]), pxf->pos);
            }
            qword d = Sub(pb->apos[i], pxf->pos);
            qword x = Cross(pxf->w, d);
            qword t = Add(MulRow(pxf->mat, pb->av[i]), pxf->v);
            pb->av[i] = Add(t, x);
        }
        if (pb->f10)
            ConvertEmitoPosVec__FP5EMITOP6VECTORT1(pemit, &pb->apos[i], &pb->av[i]);
    }
}
} // namespace M_OriginateParticles__FiP5EMITBP7EMITGEN

namespace M_OriginateSplineSinkParticles {
// OriginateSplineSinkParticles: emitter particle transform (Claude agent, lane E20). The clock time is read into a local BEFORE
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
void OriginateParticles__FiP5EMITBP7EMITGEN(int c, EMITB *pemitb, BUF *pb);
int FFloatsNear__Ffff(float g1, float g2, float gEps);
void LoadIdentityMatrix4(void *p, int n, M4 *pm0, float t, M4 *pm1);
void __ml__FRC7MATRIX4T0(M4 *pmOut, M4 *pmA, M4 *pmB);
void OriginateSplineSinkParticles(int c, EMITTER *pemit, TIMES *ptimes, BUF *pb);
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
void OriginateSplineSinkParticles(int c, EMITTER *pemit, TIMES *ptimes, BUF *pb)
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
    OriginateParticles__FiP5EMITBP7EMITGEN(c, palo->pemitb, pb);
    palo->pemitb->f1C = 0;
    tPrev = D_002A5150;
    pv = palo->pvt->pfn((ALO *)((char *)palo + palo->pvt->d), &mWorld);
    mBase = mWorld;
    for (i = 0; i < c; i++) {
        float tNow = D_002DDED0.t;
        float t = ptimes->at[i] - tNow;
        int k = palo->pemitb->k18;
        if (!FFloatsNear__Ffff(t, tPrev, 0.0001f)) {
            M4 mT;
            tPrev = t;
            LoadIdentityMatrix4(pv, 0, &m, t, 0);
            __ml__FRC7MATRIX4T0(&mT, &m, &mBase);
            m = mT;
            if (k != 3) {
                LoadIdentityMatrix4(pv, 0, &mA, t, &mB);
                __ml__FRC7MATRIX4T0(&mT, &mA, &mWorld);
                mA = mT;
                __ml__FRC7MATRIX4T0(&mT, &mB, &mWorld);
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
} // namespace M_OriginateSplineSinkParticles

namespace M_EmitParticles__FiP5EMITBP5EMITG {
class CFrame {
    int m_a[4];
public:
    CFrame() __asm__("STACKSCOPE_ctor");
    ~CFrame();
};
struct LOC { int q0, q1, q2, q3; int f; int pad[3]; };
struct L2 { int a; int b; int pad[6]; };
struct B { int pad; int k4; char pad2[0xC4 - 8]; int kC4; };
extern "C" {
void OriginateParticles__FiP5EMITBP7EMITGEN(void *a, B *b, LOC *ploc);
void EmitRips__FP5EMITBP5EMITGiP6VECTORT3PfT5(B *b, void *c, void *a, int q0, int q1, int q2, int q3);
void OriginateSplineSinkParticles(void *a, B *b, LOC *ploc, L2 *pl2);
void EmitBlips__FP5EMITBP5EMITGiP6VECTORT3PfT5T3T3(B *b, void *c, void *a, int q0, int q1, int q2, int q3, int x, int y);
/** @sly2-annot addr=0x15BD20
 * Purpose: Emits c particles: OriginateParticles, then EmitRips (rip emitters) or
 *   [OriginateSplineSinkParticles] + EmitBlips
 * Info: proposed name EmitParticles (T1); TU emitter
 * Confidence: mid (draft) [T1 0.75] - evidence: static naming pass; sources:
 *   r3_1539A8+verdicts_A1+names_v2-signature(verified)
 * See: docs/engine/particles-effects.md#6-key-functions, docs/engine/particles-effects.md#open-questions,
 *   docs/reference/functions/particles-effects.md#emitter-0x159118-0x15cd68
 * @end-sly2-annot */
void EmitParticles__FiP5EMITBP5EMITG(void *a, B *b, void *c)
{
    if (a) {
        LOC loc;
        CFrame frame;
        loc.f = 1;
        OriginateParticles__FiP5EMITBP7EMITGEN(a, b, &loc);
        if (b->k4 == 1) {
            EmitRips__FP5EMITBP5EMITGiP6VECTORT3PfT5(b, c, a, loc.q0, loc.q1, loc.q2, loc.q3);
        } else {
            L2 l2;
            if (b->kC4 == 2) {
                OriginateSplineSinkParticles(a, b, &loc, &l2);
            } else {
                l2.a = 0;
                l2.b = 0;
            }
            EmitBlips__FP5EMITBP5EMITGiP6VECTORT3PfT5T3T3(b, c, a, loc.q0, loc.q1, loc.q2, loc.q3, l2.a, l2.b);
        }
    }
}
}
} // namespace M_EmitParticles__FiP5EMITBP5EMITG

namespace M_func_0015BE28 { extern "C" {
typedef int qword __attribute__((mode(TI)));
typedef struct M4 { qword aq[4]; } M4;
typedef struct B7 { qword aq[7]; } B7;
struct X { int a; int b; int c; int pad; M4 m; B7 b7; M4 m2; };
struct Y { int a; int b; int c; };
void func_00159118(struct X *px);
void EmitParticles__FiP5EMITBP5EMITG(void *p, struct X *px, struct Y *py);
void func_002333B4(void *pv, int b, int cb);
/** @sly2-annot addr=0x15BE28
 * Purpose: Builds a temporary EMITB (InitEmitb) from explicit position/velocity/matrix/params and calls
 *   EmitParticles
 * Info: TU emitter
 * Notes: names_v2 B proposed StockSplashBig__FP6VECTORfP2SO (0.856) | no usable identifier; purpose only
 * Confidence: low (draft) [T2 0.30] - evidence: static naming pass; sources: r3_1539A8
 * @end-sly2-annot */
void func_0015BE28(qword q0, qword q1, void *p, M4 *pm, B7 *pb7, int n1, int n2, int n3)
{
    struct X x;
    struct Y y;
    func_00159118(&x);
    func_002333B4(&y, 0, sizeof(y));
    x.m.aq[2] = q0;
    x.m.aq[3] = q1;
    x.b7 = *pb7;
    x.c = n1;
    y.b = n2;
    y.c = n3;
    x.b = 1;
    x.m2 = *pm;
    EmitParticles__FiP5EMITBP5EMITG(p, &x, &y);
}
} } // namespace M_func_0015BE28

namespace M_SetEmitdvEmitb__FP6EMITDVP5EMITB { extern "C" {
typedef int qword __attribute__((mode(TI)));
struct S { char pad[0x58]; float g58; int pad5c; qword q60; qword v70; float g80; };
struct O { qword q0; qword v10; float g20; float g24; };
extern qword D_002A2DC0;
static __inline__ qword VLen(qword c)
{
    qword l, one;
    __asm__("vmul.xyz %0, %2, %2\n\tvaddw.x %1, $vf0, $vf0w\n\tvadday.x ACC, %0, %0y\n\tvmaddz.x %0, %1, %0z" : "=&j"(l), "=&j"(one) : "j"(c));
    {
    qword r;
    __asm__("vsqrt Q, %1x\n\tvwaitq\n\tvaddq.x %0, $vf0, Q" : "=j"(r) : "j"(l));
    return r;
    }
}
static __inline__ void VNormalize(qword *pr, qword c, qword k)
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
        __asm__ __volatile__("vdiv Q, $vf0w, %2x\n\tvwaitq\n\tvmulq.xyzw %0, %1, Q" : "=j"(t) : "j"(c), "j"(l));
    }
    *pr = t;
}
/** @sly2-annot addr=0x15BF40
 * Purpose: Copies the drift/velocity parameters of an EMITB into an EMITDV
 * Info: proposed name SetEmitdvEmitb (T1); TU emitter
 * Confidence: mid (draft) [T1 0.70] - evidence: static analysis; sources: r3_1539A8
 * See: docs/reference/functions/particles-effects.md#emitter-0x159118-0x15cd68
 * @end-sly2-annot */
void SetEmitdvEmitb__FP6EMITDVP5EMITB(struct O *po, struct S *ps)
{
    po->q0 = ps->q60;
    VNormalize(&po->v10, ps->v70, D_002A2DC0);
    po->g20 = ps->g80;
    po->g24 = ps->g58;
}
} } // namespace M_SetEmitdvEmitb__FP6EMITDVP5EMITB

namespace M_CalculateEmitdvMatrix__FP6EMITDVfP7MATRIX4 {
typedef int qword __attribute__((mode(TI)));
struct MATRIX4 { qword r[4]; };
struct MATRIX3 { qword r[3]; };
struct VU_FLOAT { qword q; VU_FLOAT(float f) { __asm__("" : "=r"(q) : "0"(*(int *)&f)); } };
struct CLOCK { float t; float dt; };
extern "C" { extern CLOCK D_002DDED0; }
extern "C" { extern MATRIX4 D_002A2D30; }
struct X { float x, y, z, w; qword axis; float gRot; float gScale; };
extern "C" { void func_0011B500(MATRIX3 *pm, float g, qword axis); }
MATRIX4 operator*(const MATRIX4 &a, const MATRIX4 &b) __asm__("__ml__FRC7MATRIX4T0");
MATRIX4 operator*(const MATRIX4 &m, VU_FLOAT s) __asm__("__ml__FRC7MATRIX4G8VU_FLOAT");
static inline qword Add(qword a, qword b) { qword d; __asm__("vadd.xyzw %0, %1, %2" : "=j"(d) : "j"(a), "j"(b)); return d; }
static inline MATRIX4 operator+(const MATRIX4 &a, const MATRIX4 &b) return r
{
    for (int i = 0; i < 4; i++)
        r.r[i] = Add(a.r[i], b.r[i]);
}
extern "C" MATRIX4 CalculateEmitdvMatrix__FP6EMITDVfP7MATRIX4(X *p);
/** @sly2-annot addr=0x15BFD8
 * Purpose: Computes the drift/curl matrix of an EMITDV for time t
 * Info: proposed name CalculateEmitdvMatrix (T1); TU emitter
 * Confidence: mid (draft) [T1 0.70] - evidence: static analysis; sources: r3_1539A8
 * See: docs/reference/functions/particles-effects.md#emitter-0x159118-0x15cd68
 * @end-sly2-annot */
MATRIX4 CalculateEmitdvMatrix__FP6EMITDVfP7MATRIX4(X *p)
{
    MATRIX4 mScale = D_002A2D30;
    float *pf = (float *)&mScale;
    pf[0] = p->gScale;
    pf[5] = p->gScale;
    pf[10] = p->gScale;
    pf[12] = p->x;
    pf[13] = p->y;
    pf[14] = p->z;
    MATRIX4 mRot = D_002A2D30;
    MATRIX4 *pmRot = &mRot;
    if (p->gRot != 0.0f) {
        MATRIX3 m3;
        func_0011B500(&m3, p->gRot * D_002DDED0.dt, p->axis);
        *(MATRIX3 *)pmRot = m3;
    }
    MATRIX4 mD;
    mD = *pmRot * mScale * VU_FLOAT(D_002DDED0.dt);
    return *pmRot + mD;
}
} // namespace M_CalculateEmitdvMatrix__FP6EMITDVfP7MATRIX4

namespace M_func_0015C150 { extern "C" {
typedef int qword __attribute__((mode(TI)));
struct VTE { short d; short pad; void (*pfn)(...); };
struct O { int a[3]; struct VTE *vt; };
struct P { int a; int b; float f; float g; qword q; qword r; qword s; };
void func_002333B4(void *pv, int b, int cb);
/** @sly2-annot addr=0x15C150
 * Purpose: Explodes an EXPL with default (zero) parameters via its explode virtual (slot 51)
 * Info: proposed name ExplodeExpl (T1); TU emitter
 * Confidence: mid (draft) [T1 0.75] - evidence: static analysis; sources: r3_1539A8
 * See: docs/engine/particles-effects.md#6-key-functions,
 *   docs/reference/functions/particles-effects.md#emitter-0x159118-0x15cd68
 * @end-sly2-annot */
void func_0015C150(struct O *o)
{
    struct P p = {0};
    struct VTE *vt;

    vt = o->vt;
    vt[51].pfn((char *)o + vt[51].d, &p);
}
} } // namespace M_func_0015C150

namespace M_ExplodeExplParams { extern "C" {
typedef int qword __attribute__((mode(TI)));
struct VTE { short d; short pad; void (*pfn)(...); };
struct O { int a[3]; struct VTE *vt; };
struct P { int a; int b; float f; float g; qword q; qword r; qword s; };
/** @sly2-annot addr=0x15C198
 * Purpose: Explodes an EXPL with explicit flags/reference/vector/origin/scale/radius (packs an EXPLSO, calls
 *   slot 51)
 * Info: proposed name ExplodeExplParams (T1); TU emitter
 * Confidence: mid (draft) [T1 0.75] - evidence: static analysis; sources: r3_1539A8
 * See: docs/engine/particles-effects.md#6-key-functions,
 *   docs/reference/functions/particles-effects.md#emitter-0x159118-0x15cd68
 * @end-sly2-annot */
void ExplodeExplParams(struct O *o, int a, int b, float f, float g, qword q, qword r)
{
    struct P p;
    struct VTE *e;
    p.a = a;
    p.b = b;
    p.f = f;
    p.g = g;
    p.q = q;
    p.r = r;
    e = &o->vt[51];
    e->pfn((char *)o + e->d, &p);
}
} } // namespace M_ExplodeExplParams

/** @sly2-annot addr=0x15C1E0
 * Purpose: Empty base implementation of the EXPL explode virtual (slot 51)
 * Info: proposed name ExplodeExplExplso (T1); class EXPL slot51; TU emitter
 * Confidence: mid (draft) [T1 0.50] - evidence: static naming pass; sources:
 *   r3_1539A8+verdicts_A1+names_v2-signature(verified)
 * See: docs/engine/particles-effects.md#6-key-functions,
 *   docs/reference/functions/particles-effects.md#emitter-0x159118-0x15cd68
 * @end-sly2-annot */
struct EXPL;
struct EXPLSO;
void ExplodeExplExplso(EXPL *pexpl, EXPLSO *pexplso)
{
}

namespace M_LoadExplgFromBrx { extern "C" {
typedef struct MSG { int cb; int nKind; } MSG;
void *func_001AE2A0(MSG *pmsg);
static inline void *PvAllocMsg(MSG *pmsg, int cb, int nKind) { pmsg->cb = cb; pmsg->nKind = nKind; return func_001AE2A0(pmsg); }
struct C { int a[4]; };
struct VTE { short d; short pad; void (*pfn)(...); };
struct O { int a[3]; struct VTE *vt; int pad[6]; unsigned long fl; };
struct DA { int n0; int cMax; int c; struct O **a; };
struct S { char pad[0xE0]; int c; struct O **apo; };
void STACKSCOPE_ctor(struct C *);
void STACKSCOPE_ctor_1AE4A0(struct C *, int);
void LoadXfmFromBrx(struct S *p, void *q);
void CLoadCollectorArray_ctor(struct DA *pda);
void LoadXfmChildObjectsFromBrx(struct S *p, void *q, struct DA *pda);
int FIsBasicDerivedFrom__FP5BASIC3CID(void *p, int n);
/** @sly2-annot addr=0x15C1E8
 * Purpose: EXPLG::LoadFromBrx: XFM load, reads child explosion list into apo
 * Info: proposed name LoadExplgFromBrx (T1); class EXPLG slot14; TU emitter
 * Notes: level_parser CONFIRMED-parse: payload layout followed by the parser, 100%% of bytes in 4 levels
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r3_1539A8+parse-verified
 * See: docs/engine/particles-effects.md#6-key-functions,
 *   docs/formats/level-objects.md#5-per-class-payloads-vtable-slot-14,
 *   docs/reference/functions/particles-effects.md#emitter-0x159118-0x15cd68
 * @end-sly2-annot */
void LoadExplgFromBrx(struct S *p, void *q)
{
    struct C ctx;
    struct DA da;
    MSG msg;
    int i;
    LoadXfmFromBrx(p, q);
    STACKSCOPE_ctor(&ctx);
    CLoadCollectorArray_ctor(&da);
    LoadXfmChildObjectsFromBrx(p, q, &da);
    p->apo = (__typeof__(p->apo))PvAllocMsg(&msg, da.c * 4, 0x10);
    for (i = 0; i < da.c; i++) {
        struct O *o = da.a[i];
        o->fl |= 0x200000000000000L;
        if (!FIsBasicDerivedFrom__FP5BASIC3CID(o, 0x1E)) {
            struct VTE *e = &o->vt[9];
            e->pfn((char *)o + e->d);
        } else {
            p->apo[p->c++] = o;
        }
    }
    STACKSCOPE_ctor_1AE4A0(&ctx, 2);
}
} } // namespace M_LoadExplgFromBrx

/** @sly2-annot addr=0x15C310
 * Purpose: EXPLG: returns child count (+0xE0) and child array (+0xE4) via out-param
 * Info: proposed name CploExplgChildren (T1); class EXPLG slot28; TU emitter
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: r3_1539A8
 * See: docs/reference/functions/particles-effects.md#emitter-0x159118-0x15cd68
 * @end-sly2-annot */
extern "C" int CploExplgChildren(EXPLG *pexplg, LO ***paplo)
{
    *paplo = pexplg->aplo;
    return pexplg->cplo;
}

namespace M_CloneExplg { extern "C" {
typedef struct MSG { int cb; int nKind; } MSG;
void *func_001AE238(MSG *pmsg, void *pv);
static inline void *PvReallocMsg(MSG *pmsg, void *pv, int cb, int nKind) { pmsg->cb = cb; pmsg->nKind = nKind; return func_001AE238(pmsg, pv); }
struct S { int pad[4]; void *p10; char pad2[0xE0 - 0x14]; int c; void **apv; };
void CloneXfm(struct S *p, void *pv);
void *PloCloneLo__FP2LOP2SO(void *pv, void *p10);
/** @sly2-annot addr=0x15C320
 * Purpose: EXPLG clone: XFM clone, realloc child array, clone each child (0x13E510)
 * Info: proposed name CloneExplg (T1); class EXPLG slot41; TU emitter
 * Confidence: mid (draft) [T1 0.70] - evidence: static analysis; sources: r3_1539A8
 * See: docs/reference/functions/particles-effects.md#emitter-0x159118-0x15cd68
 * @end-sly2-annot */
void CloneExplg(struct S *p, void *pv)
{
    MSG msg;
    int i;
    CloneXfm(p, pv);
    p->apv = (__typeof__(p->apv))PvReallocMsg(&msg, p->apv, p->c * 4, 0x10);
    for (i = 0; i < p->c; i++)
        p->apv[i] = PloCloneLo__FP2LOP2SO(p->apv[i], p->p10);
}
} } // namespace M_CloneExplg

namespace M_ExplodeExplgExplso {
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
extern "C" { MATRIX4 InvertMatrix4Rigid(MATRIX4 *pm); }
MATRIX4 operator*(const MATRIX4 &a, const MATRIX4 &b) __asm__("__ml__FRC7MATRIX4T0");
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
} // namespace M_ExplodeExplgExplso

namespace M_HandleExplsMessage { extern "C" {
struct O { char pad[0x100]; void *p100; void *p104; };
void UnsubscribeLoObject(void *pv, void *po);
/** @sly2-annot addr=0x15C5C8
 * Purpose: EXPLS::HandleMessage: on msg 1 (removed) forgets cached p100/p104 references
 * Info: proposed name HandleExplsMessage (T1); class EXPLS slot5; TU emitter
 * Notes: names_v2 B proposed BindExplg__FP5EXPLG (0.856)
 * Confidence: mid (draft) [T1 0.70] - evidence: static naming pass; sources: r3_1539A8
 * See: docs/reference/functions/particles-effects.md#emitter-0x159118-0x15cd68
 * @end-sly2-annot */
void HandleExplsMessage(struct O *o, int msgid, void *pv)
{
    if (msgid == 1) {
        if (pv == o->p100) {
            UnsubscribeLoObject(pv, o);
            o->p100 = 0;
        } else if (pv == o->p104) {
            UnsubscribeLoObject(pv, o);
            o->p104 = 0;
        }
    }
}
} } // namespace M_HandleExplsMessage

namespace M_ExplodeExplsExplso { extern "C" {
struct VTE { short d; short pad; void (*pfn)(...); };
struct O { int a[3]; struct VTE *vt; };
/** @sly2-annot addr=0x15C628
 * Purpose: EXPLS explode: forwards to its own vtable slot 52 (fire)
 * Info: proposed name ExplodeExplsExplso (T1); class EXPLS slot51; TU emitter
 * Confidence: mid (draft) [T1 0.55] - evidence: static analysis; sources: r3_1539A8+verdicts_B
 * See: docs/engine/particles-effects.md#6-key-functions,
 *   docs/reference/functions/particles-effects.md#emitter-0x159118-0x15cd68
 * @end-sly2-annot */
void ExplodeExplsExplso(struct O *o, int x)
{
    struct VTE *e = &o->vt[52];
    e->pfn((char *)o + e->d, x);
}
} } // namespace M_ExplodeExplsExplso

/** @sly2-annot addr=0x15C650
 * Purpose: UNKNOWN - no analysis recorded yet (byte-matched only).
 * Info: TU emitter
 * Notes: names_v2 v1 hint not used: CloneExplo__FP5EXPLOT0
 * Confidence: none - unanalysed; anything inferred from the code here is a guess
 * @end-sly2-annot */
extern "C" SFX *func_0015C650(SFXHOLDER *p)
{
    if (p->psfx == 0) {
        p->psfx = NewSfx();
    }
    return p->psfx;
}

namespace M_FireExplsExplso { extern "C" {
// (Claude agent) MATCH: g8 if/else reload form, max via local h=0.5f, pfrm transform split into mul asm + vadd asm with t loaded late.
typedef int qword __attribute__((mode(TI)));
typedef struct EMITP { int x0; int x4; int x8; int xC; int x10; } EMITP;
typedef struct XF4 { qword v; } XF4;
typedef struct X4 { char pad[0xF4]; XF4 *p; } X4;
typedef struct FRM { qword pos; qword m[3]; } FRM;
typedef struct EMITB {
    int x0; int x4; EMITP *pemitp; int xC; int x10; int x14; int x18; FRM *pfrm; int pad20[4];
    union { qword v; qword *pm; EMITP *pemitp; struct { char pad[0x20]; qword v; } *pq; } u30;
    qword v40;
    float g50, g54;
    char pad58[0xD8 - 0x58];
    float gD8, gDC;
    qword vE0[2];
} EMITB;
typedef struct SND { int id; float g4, g8, gC, g10; } SND;
typedef struct EMITTER { char pad[0xE4]; EMITB *pemitb; int xE8, xEC; SND *psnd; float gF4, gF8; int xFC; int x100; int x104; } EMITTER;
typedef struct EMITO { int grf; int x4; float g8; float gC; qword v10; qword v20; int x30; int x34; int x38; } EMITO;
typedef struct EMITR { int *p104; int *p100; EMITTER *p; } EMITR;

extern char D_002BDEC0[];
extern float GRandInRange__Fff(float lo, float hi);
extern float GLimitLm__FP2LMf(void *p, float g);
typedef union V4 { qword q; float a[4]; } V4;
extern V4 func_00256CE8(float g);
extern V4 __9VU_VECTORRC6VECTOR(qword *pv);
extern qword __ml__FG8VU_FLOATG9VU_VECTOR(qword a, qword b);
extern void VECTOR_opAssign(qword *pv, qword a);
extern void EmitParticles__FiP5EMITBP5EMITG(int c, EMITB *pb, EMITR *pr);
extern qword ConvertAloPos(int x, int y, qword v);
extern void *PsndStartSound(int id);
extern void SetSndSourceLo(void *p, void *pso);
extern void SetSndSourcePos(void *p, qword v);
extern void SetSndRange(void *p, float a, float b);
extern void func_001FB1F8(void *p, float g);
extern void SetSndPitch(void *p, float g);

/** @sly2-annot addr=0x15C690
 * Purpose: EXPLS fire: builds a temporary emitter setup (EMITP/EMITB/EMITR copies) at the explosion position
 *   and emits particles
 * Info: proposed name FireExplsExplso (T1); class EXPLS slot52; TU emitter
 * Confidence: mid (draft) [T1 0.55] - evidence: static analysis; sources: r3_1539A8
 * See: docs/reference/functions/particles-effects.md#emitter-0x159118-0x15cd68
 * @end-sly2-annot */
void FireExplsExplso(EMITTER *p, EMITO *po)
{
    EMITP emitp;
    EMITB b;
    EMITR r;
    EMITR *pr;
    int c;
    V4 pos;
    V4 h;

    emitp = *p->pemitb->pemitp;
    b = *p->pemitb;
    b.pemitp = &emitp;
    if (po->grf & 0x80)
        c = po->x38;
    else
        c = (int)GRandInRange__Fff(p->gF4, p->gF8);
    if (po->grf & 1)
        emitp.x0 = po->x4;
    if (po->grf & 2)
        b.v40 = po->v10;
    if (po->grf & 4) {
        b.u30.v = po->v20;
        b.x10 = 0;
    } else if (po->grf & 0x40) {
        b.pfrm = (FRM *)po->x34;
    }
    if ((po->grf & 8) && b.x4 == 0) {
        float g;
        float gs;
        int n;
        if (po->g8 == 0.0f)
            g = po->gC * 0.01f;
        else
            g = po->g8;
        if (g < 1.0f)
            c = (int)(c * g);
        else
            c = (int)(c / g);
        n = (int)p->gF8 / 2;
        if (!(n < c))
            c = n;
        gs = GLimitLm__FP2LMf(D_002BDEC0, g);
        b.g50 *= gs;
        b.g54 *= gs;
        { float h = 0.5f; g = (h < g) ? g : h; }
        VECTOR_opAssign(b.vE0, __ml__FG8VU_FLOATG9VU_VECTOR(func_00256CE8(g).q, __9VU_VECTORRC6VECTOR(b.vE0).q));
    }
    if (b.x4 == 1) {
        if (po->grf & 0x10)
            b.gDC = b.gD8 = po->g8;
        if (po->grf & 0x20)
            b.pemitp->x10 = po->x30;
    }
    if (p->xFC) {
        r.p104 = &p->x104;
        r.p100 = &p->x100;
        r.p = p;
        pr = &r;
    } else {
        pr = 0;
    }
    if (emitp.x0 == 0 && b.x10 != 2 && !(po->grf & 0x48))
        emitp.x0 = (int)p;
    EmitParticles__FiP5EMITBP5EMITG(c, &b, pr);
    if (p->psnd) {
        void *ps;
        switch (b.x10) {
        case 0:
            pos.q = b.u30.v;
            break;
        case 1: {
            float half = 0.5f;
            {
                register qword hv __asm__("vf1");
                __asm__("qmtc2.ni %1, %0" : "=j"(hv) : "r"(half));
                h.q = hv;
                {
                    qword *pm = b.u30.pm;
                    register qword m1 __asm__("vf2") = pm[1];
                    register qword m0 __asm__("vf3") = pm[0];
                    register qword r __asm__("vf1");
                    __asm__("vmulax.xyzw $ACC, %1, %3x\n\tvmaddx.xyzw %0, %2, %3x" : "=j"(r) : "j"(m0), "j"(m1), "j"(hv));
                    pos.q = r;
                }
            }
            break;
        }
        case 2:
            pos.q = ((X4 *)b.pemitp->x4)->p->v;
            break;
        case 3:
            pos.q = b.u30.pq->v;
            break;
        }
        if (b.pfrm) {
            FRM *pf = b.pfrm;
            qword *pm = pf->m;
            register qword v __asm__("vf3") = pos.q;
            register qword m0 __asm__("vf4") = pm[0];
            register qword m1 __asm__("vf1") = pm[1];
            register qword m2 __asm__("vf5") = pm[2];
            __asm__("vmulax.xyz $ACC, %2, %1x\n\tvmadday.xyz $ACC, %3, %1y\n\tvmaddz.xyz %0, %4, %1z"
                : "=j"(v) : "0"(v), "j"(m0), "j"(m1), "j"(m2));
            {
            register qword t __asm__("vf2") = pf->pos;
            __asm__("vadd.xyzw %0, %0, %1" : "=j"(v) : "j"(t), "0"(v));
            }
            pos.q = v;
        }
        pos.q = ConvertAloPos(emitp.x0, 0, pos.q);
        ps = PsndStartSound(p->psnd->id);
        SetSndSourceLo(ps, 0);
        SetSndSourcePos(ps, pos.q);
        SetSndRange(ps, p->psnd->g4, p->psnd->g8);
        func_001FB1F8(ps, p->psnd->gC);
        SetSndPitch(ps, p->psnd->g10);
    }
}
} } // namespace M_FireExplsExplso



namespace M_LoadEmitblipColorsFromBrx { extern "C" {
typedef struct { unsigned char r, g, b, a; } RGBA;
typedef struct { int n; RGBA ac[1]; } LIST;
typedef struct { char pad[0xC]; int xC; char pad2[0xC]; LIST *x1C; } Y;
typedef struct { char pad[0xC0]; Y y; } T;
typedef struct { char pad[0xE4]; T *xE4; } X;
typedef struct { int cb; int align; } DESC;
int U16Read__18CBinaryInputStream(void *);
int U8Read__18CBinaryInputStream(void *);
int U32Read__18CBinaryInputStream(void *);
void ColorCorrectRgba(RGBA *);
void *func_001AE1D0(DESC *);

static inline int Min(int a, int b) { return a < b ? a : b; }

/** @sly2-annot addr=0x2584A8
 * Purpose: read count + RGBA list into EMITB [game linkonce (inline/template)]
 * Info: proposed name LoadEmitblipColorsFromBrx (T1); TU game_linkonce
 * Notes: level_parser CONFIRMED-parse: payload layout followed by the parser, 100%% of bytes in 4 levels
 * Confidence: mid (draft) [T1 0.75] - evidence: static analysis; sources: sdk_names+parse-verified
 * See: docs/engine/core-library.md#7-key-functions,
 *   docs/reference/functions/core-library.md#game_linkonce-0x254f00-0x259de4
 * @end-sly2-annot */
void LoadEmitblipColorsFromBrx(X *p, void *ps)
{
    int n = U16Read__18CBinaryInputStream(ps);
    int i;
    Y *y;
    LIST *l;
    union { DESC d; RGBA c; } u;
    if (n == 0)
        return;
    y = &p->xE4->y;
    u.d.cb = n * 4 + 4;
    u.d.align = 0x10;
    l = (__typeof__(l))func_001AE1D0(&u.d);
    y->x1C = l;
    y->xC = U8Read__18CBinaryInputStream(ps);
    l->n = Min(n, 32);
    for (i = 0; i < n; i++) {
        *(int *)&u.c = U32Read__18CBinaryInputStream(ps);
        ColorCorrectRgba(&u.c);
        if (i < l->n)
            l->ac[i] = u.c;
    }
}
} } // namespace M_LoadEmitblipColorsFromBrx

namespace M_LoadEmitterFromBrx { extern "C" {
typedef struct { char pad[0x10]; int x10; char pad2[0x1C]; void *x30; } E;
typedef struct { char pad[0xE4]; E *xE4; } S;
typedef struct { int a, b; } P;
E *func_001AE1D0(P *); void func_00159118(E *); void LoadXfmFromBrx(S *, int); void LoadEmitmeshFromBrx__FP8EMITMESHP18CBinaryInputStream(void *, int); void LoadEmitblipColorsFromBrx(S *, int);
void LoadEmitterFromBrx(S *p, int x)
{
    P t;
    t.a = 0x100; t.b = 0x10;
    func_00159118(p->xE4 = func_001AE1D0(&t));
    LoadXfmFromBrx(p, x);
    if (p->xE4->x10 == 3) LoadEmitmeshFromBrx__FP8EMITMESHP18CBinaryInputStream(p->xE4->x30, x);
    LoadEmitblipColorsFromBrx(p, x);
}
} } // namespace M_LoadEmitterFromBrx

/** @sly2-annot addr=0x258620
 * Purpose: EMITTER vt41: CloneXfm then share EMITB (cref++) [game linkonce (inline/template)]
 * Info: proposed name CloneEmitter (T1); class EMITTER slot41; TU game_linkonce
 * Confidence: mid (draft) [T1 0.55] - evidence: static analysis; sources: sdk_names
 * See: docs/reference/functions/core-library.md#game_linkonce-0x254f00-0x259de4
 * @end-sly2-annot */
/* linkonce copy (0x258620): XFM clone, then one more share of the emitter block. */
extern "C" void CloneEmitter(EMITTER *pemitter, EMITTER *pemitterBase)
{
    CloneXfm(pemitter, pemitterBase);
    pemitter->pemitb->cref++;
}

namespace M_func_002586D8 { extern "C" {
typedef struct { int cb; int cbAlign; } AREQ;
typedef struct {
    int x0; int x4; void *x8; int pad0C; int x10; int pad14[3]; int x20; int pad24[3]; int x30;
    char p34[0xC8 - 0x34]; int xC8; int xCC; int xD0; int xD4;
} X;
typedef struct { char p0[0xE4]; X *px; } SO;
extern char D_003DD490[];
extern char D_002CE4A0[];
void PostLoLoad(SO *p);
void *func_002333B4(void *p, int c, int n);
int func_00233178(const void *a, const void *b, int n);
void *PloFindSwObject__FP2SWi3OIDP2LO(void *pg, int kind, int oid, SO *p);
void PemitbEnsureEmitter(SO *p, int f);
void *func_001AE238(AREQ *preq, void *pv);
/** @sly2-annot addr=0x2586D8
 * Purpose: resolve emitter object references (PloFindSwObject) after load [game linkonce (inline/template)]
 * Info: proposed name BindEmitter (T2); TU game_linkonce
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: sdk_names
 * See: docs/reference/functions/core-library.md#game_linkonce-0x254f00-0x259de4
 * @end-sly2-annot */
void func_002586D8(SO *p)
{
    void *a[5];
    AREQ req;
    AREQ *preq;
    X *x;
    int o1;
    int o2 = -1;

    PostLoLoad(p);
    {
        X *x0 = p->px;
        o1 = x0->x20;
        if (x0->x10 == 2)
            o2 = x0->x30;
    }
    func_002333B4(a, 0, 0x14);
    if (o1 != -1)
        a[0] = PloFindSwObject__FP2SWi3OIDP2LO(D_003DD490, 4, o1, p);
    if (o2 != -1) {
        void *q = PloFindSwObject__FP2SWi3OIDP2LO(D_003DD490, 4, o2, p);
        a[0] = 0;
        a[1] = q;
    }
    x = p->px;
    switch (x->x4) {
    case 1: {
        int c8 = x->xC8;
        int cc = x->xCC;
        int d0 = x->xD0;
        if (c8 != -1)
            a[2] = PloFindSwObject__FP2SWi3OIDP2LO(D_003DD490, 4, c8, p);
        if (cc != -1)
            a[3] = PloFindSwObject__FP2SWi3OIDP2LO(D_003DD490, 4, cc, p);
        if (d0 != -1)
            a[4] = PloFindSwObject__FP2SWi3OIDP2LO(D_003DD490, 4, d0, p);
        break;
    }
    case 0:
        if (x->xD4 != -1)
            a[2] = PloFindSwObject__FP2SWi3OIDP2LO(D_003DD490, 4, x->xD4, p);
        break;
    }
    if (p->px->x8 == 0)
        p->px->x8 = D_002CE4A0;
    if (func_00233178(p->px->x8, a, 0x14)) {
        PemitbEnsureEmitter(p, 1);
        preq = &req;
        req.cb = 0x14;
        preq->cbAlign = 0x10;
        p->px->x8 = func_001AE238(preq, a);
    }
}
} } // namespace M_func_002586D8

namespace M_LoadExploBlipColorsFromBrx { extern "C" {
typedef struct { unsigned char r, g, b, a; } RGBA;
typedef struct { int n; RGBA ac[1]; } LIST;
typedef struct { char pad[0xC]; int xC; char pad2[0xC]; LIST *x1C; } Y;
typedef struct { char pad[0xC0]; Y y; } T;
typedef struct { char pad[0xE4]; T *xE4; } X;
typedef struct { int cb; int align; } DESC;
int U16Read__18CBinaryInputStream(void *);
int U8Read__18CBinaryInputStream(void *);
int U32Read__18CBinaryInputStream(void *);
void ColorCorrectRgba(RGBA *);
void *func_001AE1D0(DESC *);

static inline int Min(int a, int b) { return a < b ? a : b; }

/** @sly2-annot addr=0x258F40
 * Purpose: EXPLO twin of 0x2584A8 [game linkonce (inline/template)]
 * Info: proposed name LoadExploBlipColorsFromBrx (T1); TU game_linkonce
 * Notes: level_parser CONFIRMED-parse: payload layout followed by the parser, 100%% of bytes in 4 levels
 * Confidence: mid (draft) [T1 0.75] - evidence: static analysis; sources: sdk_names+parse-verified
 * See: docs/engine/core-library.md#7-key-functions,
 *   docs/reference/functions/core-library.md#game_linkonce-0x254f00-0x259de4
 * @end-sly2-annot */
void LoadExploBlipColorsFromBrx(X *p, void *ps)
{
    int n = U16Read__18CBinaryInputStream(ps);
    int i;
    Y *y;
    LIST *l;
    union { DESC d; RGBA c; } u;
    if (n == 0)
        return;
    y = &p->xE4->y;
    u.d.cb = n * 4 + 4;
    u.d.align = 0x10;
    l = (__typeof__(l))func_001AE1D0(&u.d);
    y->x1C = l;
    y->xC = U8Read__18CBinaryInputStream(ps);
    l->n = Min(n, 32);
    for (i = 0; i < n; i++) {
        *(int *)&u.c = U32Read__18CBinaryInputStream(ps);
        ColorCorrectRgba(&u.c);
        if (i < l->n)
            l->ac[i] = u.c;
    }
}
} } // namespace M_LoadExploBlipColorsFromBrx

namespace M_LoadExploFromBrx { extern "C" {
typedef struct { char pad[0x10]; int x10; char pad2[0x1C]; void *x30; } E;
typedef struct { char pad[0xE4]; E *xE4; } S;
typedef struct { int a, b; } P;
E *func_001AE1D0(P *); void func_00159118(E *); void LoadXfmFromBrx(S *, int); void LoadEmitmeshFromBrx__FP8EMITMESHP18CBinaryInputStream(void *, int); void LoadExploBlipColorsFromBrx(S *, int);
void LoadExploFromBrx(S *p, int x)
{
    P t;
    t.a = 0x100; t.b = 0x10;
    func_00159118(p->xE4 = func_001AE1D0(&t));
    LoadXfmFromBrx(p, x);
    if (p->xE4->x10 == 3) LoadEmitmeshFromBrx__FP8EMITMESHP18CBinaryInputStream(p->xE4->x30, x);
    LoadExploBlipColorsFromBrx(p, x);
}
} } // namespace M_LoadExploFromBrx

/** @sly2-annot addr=0x2590B8
 * Purpose: EXPLO/EXPLS vt41 [game linkonce (inline/template)]
 * Info: proposed name CloneExplo (T1); class EXPLO slot41; TU game_linkonce
 * Confidence: mid (draft) [T1 0.55] - evidence: static analysis; sources: sdk_names
 * See: docs/reference/functions/core-library.md#game_linkonce-0x254f00-0x259de4
 * @end-sly2-annot */
/* linkonce copy (0x2590B8), the EXPLO instance of the same code (EXPLO's block pointer is also at +0xE4). */
extern "C" void CloneExplo(EMITTER *pexplo, EMITTER *pexploBase)
{
    CloneXfm(pexplo, pexploBase);
    pexplo->pemitb->cref++;
}

namespace M_PemitbEnsureExplo { extern "C" {
typedef struct { int x0; } E;
typedef struct { char pad[0xE0]; int xE0; E *xE4; } S;
typedef struct { int a, b; } P;
E *func_001AE238(P *, E *);
E *PemitbEnsureExplo(S *p, int x)
{
    if (x == 1) {
        p->xE0 = x;
        if (p->xE4->x0 >= 2) {
            P t;
            p->xE4->x0--;
            t.a = 0x100; t.b = 0x10;
            p->xE4 = func_001AE238(&t, p->xE4);
            p->xE4->x0 = x;
        }
    }
    return p->xE4;
}
} } // namespace M_PemitbEnsureExplo

namespace M_func_00259170 { extern "C" {
typedef struct { int cb; int cbAlign; } AREQ;
typedef struct {
    int x0; int x4; void *x8; int pad0C; int x10; int pad14[3]; int x20; int pad24[3]; int x30;
    char p34[0xC8 - 0x34]; int xC8; int xCC; int xD0; int xD4;
} X;
typedef struct { char p0[0xE4]; X *px; } SO;
extern char D_003DD490[];
extern char D_002CE4B8[];
void PostLoLoad(SO *p);
void *func_002333B4(void *p, int c, int n);
int func_00233178(const void *a, const void *b, int n);
void *PloFindSwObject__FP2SWi3OIDP2LO(void *pg, int kind, int oid, SO *p);
void PemitbEnsureExplo(SO *p, int f);
void *func_001AE238(AREQ *preq, void *pv);
/** @sly2-annot addr=0x259170
 * Purpose: EXPLO vt15 PostLoad: resolve object refs [game linkonce (inline/template)]
 * Info: proposed name BindExplo (T2); class EXPLO slot15; TU game_linkonce
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: sdk_names
 * See: docs/reference/functions/core-library.md#game_linkonce-0x254f00-0x259de4
 * @end-sly2-annot */
void func_00259170(SO *p)
{
    void *a[5];
    AREQ req;
    AREQ *preq;
    X *x;
    int o1;
    int o2 = -1;

    PostLoLoad(p);
    {
        X *x0 = p->px;
        o1 = x0->x20;
        if (x0->x10 == 2)
            o2 = x0->x30;
    }
    func_002333B4(a, 0, 0x14);
    if (o1 != -1)
        a[0] = PloFindSwObject__FP2SWi3OIDP2LO(D_003DD490, 4, o1, p);
    if (o2 != -1) {
        void *q = PloFindSwObject__FP2SWi3OIDP2LO(D_003DD490, 4, o2, p);
        a[0] = 0;
        a[1] = q;
    }
    x = p->px;
    switch (x->x4) {
    case 1: {
        int c8 = x->xC8;
        int cc = x->xCC;
        int d0 = x->xD0;
        if (c8 != -1)
            a[2] = PloFindSwObject__FP2SWi3OIDP2LO(D_003DD490, 4, c8, p);
        if (cc != -1)
            a[3] = PloFindSwObject__FP2SWi3OIDP2LO(D_003DD490, 4, cc, p);
        if (d0 != -1)
            a[4] = PloFindSwObject__FP2SWi3OIDP2LO(D_003DD490, 4, d0, p);
        break;
    }
    case 0:
        if (x->xD4 != -1)
            a[2] = PloFindSwObject__FP2SWi3OIDP2LO(D_003DD490, 4, x->xD4, p);
        break;
    }
    if (p->px->x8 == 0)
        p->px->x8 = D_002CE4B8;
    if (func_00233178(p->px->x8, a, 0x14)) {
        PemitbEnsureExplo(p, 1);
        preq = &req;
        req.cb = 0x14;
        preq->cbAlign = 0x10;
        p->px->x8 = func_001AE238(preq, a);
    }
}
} } // namespace M_func_00259170

/** @sly2-annot addr=0x2599C8
 * Purpose: duplicate EXPLO class-descriptor getter (vtable slot 6 of the second, never-stored EXPLO vtable):
 *   returns the same record as GetExploClassDesc (0x113C10)
 * Info: was proposed as EXPLO_PvOptionDefaults (wrong); _linkonce suffix keeps the symbol distinct from
 *   GetExploClassDesc; TU game_linkonce
 * Confidence: high - evidence: static analysis (same data record as GetExploClassDesc); sources:
 *   class_reference hidden-types
 * See: docs/reference/functions/core-library.md#game_linkonce-0x254f00-0x259de4
 * @end-sly2-annot */
extern "C" {
extern char D_002BAD68[];   /* EXPLO class descriptor */
}
/* linkonce copy (0x2599C8). */
extern "C" void *GetExploClassDesc_linkonce()
{
    return D_002BAD68;
}
