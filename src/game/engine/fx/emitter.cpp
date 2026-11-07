// emitter.cpp -- particle emitters (EMITTER : XFM) and explosions (EXPL/EXPLG/EXPLS/EXPLO): emitter blocks (EMITB),
// origins (EMITO) and velocities (EMITV), emission of particles / blips / rips, the emitter update and pause, and
// the explosion groups. TU 0x159118..0x15CD68 (44 functions, address order); at the end, the explicit
// instantiations of sly2/emitter.h's member templates (the 12 linkonce copies 0x2584A8..0x2599C8).
//
// Types: EMITTER and EMITB are sly2/emitter.h's (the fields the m2c bodies read are named there); the 0x70-byte
// EMITV defaults below are EMITDEF::EMITV, a different record from emitter.h's 0x40-byte EMITV.
// The hand-written per-function files keep their views in namespaces named after each function (as jt.cpp does);
// their C++-named definitions are C-linkage definitions of their literal symbols there (the namespace would change
// the mangling), as in wave 9.
// The static initializer (StaticInit_emitter, stub func_0015CD48) is generated from the two emitter defaults and the six
// slot pools below.
// SLY2_ALIAS: __static_initialization_and_destruction_0=StaticInit_emitter
// SLY2_ALIAS: _GLOBAL_$I$*=func_0015CD48

#include "sly2/basic.h"
#include "sly2/scrap.h"
#include "sly2/xfm.h"
#include "sly2/slotheap.h"
#include "sly2/stackscope.h"
#include "sly2/emitter.h"
#include "sly2/classes.h"
#include "sly2/vu0.h"
#include "sly2/util.h"
extern "C" char base_D_002BAAE8[] __asm__("D_002BAAE8");   /* base of the address names below: see docs/guides/tu-merging.md "Data" */

/* The emitter defaults (0x15CAD8, the static initializer). D_002DE5B0 / D_002DE5F0 are the EMITO / EMITV defaults
   that InitEmitBlock (InitEmitb) copies into EMITB+0x10 / +0x50 (Sly 1's InitEmitb builds the same two objects as
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
/* The origin record (EMITB+0x10; Sly 1's EMITO starts the same way). */
struct EMITO {
    int kind[4];            /* +0x00 EMITOK (0 here: a point), padding */
    int originOid;          /* +0x10 the origin object (-1: none) */
    int pad14[3];
    QWV position;           /* +0x20 point origin (other kinds: their data pointer) */
    qword direction;        /* +0x30 emission direction (default +x) */
};
/* The velocity record (EMITB+0x50); meanings from Sly 1's EMITV and its InitEmitb defaults, which have the same
   values in the same order. */
struct EMITV {
    G4V speed;              /* +0x00 g0/g4: speed range; g8: velocity damping (-1); gC: z speed ratio (1: the
                               emission direction's component is scaled by it, ChooseEmitvVelocityAge) */
    int addedVelocityX, addedVelocityY;   /* +0x10 added to every particle's velocity (default 0,0,-1500: gravity) */
    float addedVelocityZ;
    int curl;               /* +0x1C */
    qword curlAxis;         /* +0x20 */
    int curlKind;           /* +0x30 */
    int particlesPerRing;   /* +0x34 */
    float randomRadius, lifetime;   /* +0x38, +0x3C */
    int tiltSkipBirth[8];   /* +0x40 tilt range (radians), skip and birth time ranges */
    CLQU alpha;             /* +0x60 alpha over the particle's life */
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
/* EMITTER emission modes (EMITTER+0xF0; Sly 1's EMITRK has the same four values; the names here follow what Sly 2's
   update does with them). */
enum EMIT_MODE {
    EMIT_MODE_UNSET = -1,           /* PostEmitterLoad infers one */
    EMIT_MODE_CONTINUOUS = 0,       /* `rate` particles per second until the count runs out */
    EMIT_MODE_CONSTANT_COUNT = 1,   /* keep `constantCount` particles alive */
    EMIT_MODE_BURST = 2,            /* `rate` particles at once, then maybe pause */
    EMIT_MODE_INTERMITTENT = 3,     /* `rate` particles per minute(?) as a stream, pausing at random (inferred) */
};
/* EMITO origin kinds (EMITB+0x10) and how the emission direction is chosen (EMITB+0x14; Sly 1's EMITNK) */
enum EMIT_ORIGIN { EMIT_ORIGIN_POINT = 0, EMIT_ORIGIN_BOX = 1, EMIT_ORIGIN_CURVE = 2, EMIT_ORIGIN_MESH = 3 };
enum EMIT_NORMAL {
    EMIT_NORMAL_VECTOR = 0,         /* the record's vector */
    EMIT_NORMAL_RADIAL = 1,         /* away from the vector (a centre point) */
    EMIT_NORMAL_CURVE_TANGENT = 2,
    EMIT_NORMAL_CURVE_NORMAL = 3,   /* tangent x the vector */
    EMIT_NORMAL_SCREEN = 4,         /* the vector in camera space */
    EMIT_NORMAL_MESH_NORMAL = 5,
};
/* how particle velocities follow the origin's reference object (EMITB+0x18; Sly 1's EMITVK) */
enum EMIT_VELOCITY {
    EMIT_VELOCITY_RELATIVE = 0,         /* rotated, plus the object's velocity at the origin */
    EMIT_VELOCITY_RELATIVE_SPIN = 1,    /* rotated, plus the object's velocity at the particle's position */
    EMIT_VELOCITY_NO_RELATIVE = 2,      /* rotated only */
    EMIT_VELOCITY_WORLD = 3,            /* not transformed */
};
/* EMITB and EMITTER (CID 0x1D : XFM, cb 0x160): sly2/emitter.h. The V names data/vtables gave the file-scope views
   (classes.h is included) stay as aliases of the real types, so the mangled names do not change. */
typedef EMITB EMITBV;
typedef EMITTER EMITTERV;
/* EXPLSO flags: which of the explode request's fields the caller set (Sly 1's GRFEXPLSO) */
enum EXPLSO_FLAGS {
    EXPLSO_HAS_REFERENCE = 0x01,
    EXPLSO_HAS_VECTOR = 0x02,
    EXPLSO_HAS_ORIGIN = 0x04,
    EXPLSO_HAS_SCALE = 0x08,
    EXPLSO_HAS_RIP_SCALE = 0x10,    /* rips: the scale is their fixed initial scale */
    EXPLSO_HAS_TOUCH = 0x20,
    EXPLSO_HAS_XF = 0x40,           /* a local transform for the origin */
    EXPLSO_HAS_COUNT = 0x80,
};
/* EXPLG (an explosion group, : EXPL : XFM): its children. */
struct EXPLGV : public XFM
{
    int numChildren;        /* +0xE0 */
    LO **children;          /* +0xE4 */
};
/* The EXPLS (an explosion's sound-effect part; EnsureExplsSfx is registered as an EXPLS option accessor in the class
   option table): its lazily created SFX at +0xF0. */
struct SFXHOLDER : public XFM
{
    char padE0[0x10];
    SFX *sfx;               /* +0xF0 */
};

extern SW D_003DD490;       /* g_sw */
extern "C" {
extern const float D_002A514C;    /* "indefinitely": a time far in the future */
extern CLOCK D_002DDED0;    /* g_clock */
void AddSwPoFollower(SW *psw, EMITTERV *pemitter);
void SetLoParent(LO *plo, SO *psoParent, int n);
SO *GetSoFromLo(int oid);
void SetBlipgEmitb(BLIPG *pblipg, EMITBV *pemitb);
SFX *NewSfx();
void CloneXfm(XFM *pxfm, XFM *pxfmBase);
}
void SetRipgEmitb(RIPG *pripg, EMITBV *pemitb) __asm__("SetRipgEmitb__FP4RIPGP5EMITB");
float RandomFloatInRange(float gMin, float gMax);

namespace M_InitEmitBlock { extern "C" {
typedef int qword __attribute__((mode(TI)));
typedef struct M4 { qword aq[4]; } M4;
typedef struct B7 { qword aq[7]; } B7;
extern M4 D_002DE5B0;
extern B7 D_002DE5F0;
struct S { int cref; int particleKind; int pad[2]; M4 origin; B7 velocity; char particle[0x100 - 0xC0]; };   /* EMITB */
void *func_002333B4(void *pv, int b, int cb) __asm__("memset");
// Zeroes an EMITB and installs the default EMITO/EMITV objects (D_002DE5B0/D_002DE5F0)
void InitEmitBlock(struct S *p)
{
    func_002333B4(p, 0, sizeof(struct S));
    p->cref = 1;
    p->particleKind = -1;
    p->origin = D_002DE5B0;
    p->velocity = D_002DE5F0;
}
} } // namespace M_InitEmitBlock

namespace M_InitEmitter { extern "C" {
void InitXfm(void *p);
void InitDl__FP2DLi(void *pdl, int ib);
void SetLoBusyKind(void *p, int n);
} } // namespace M_InitEmitter
// EMITTER::Init: XFM init, then "unset" defaults (mode and count -1, no group, 10 particles per second, not paused),
// an empty slaved-emitter list linked through +0x128, busy level 0, and LO flag bit 2 set (field meanings from Sly 1's
// EMITTER, which has the same members in the same order).
void EMITTER::Init()
{
    unsigned int flags;
    M_InitEmitter::InitXfm(this);
    particleCount = -1;
    emitMode = -1;
    groupOid = -1;
    rate.maxValue = 10.0f;
    rate.minValue = 10.0f;
    tUnpause = -1.0f;
    M_InitEmitter::InitDl__FP2DLi(&groupList, 0x128);
    M_InitEmitter::SetLoBusyKind(this, 0);
    flags = unknown40;   /* LO flag word, or (bit 0 clear) a pointer to the shared one */
    if ((flags ^ 1) & 1)
        *(int *)flags |= 2;     /* bit 0 clear: the word points at the shared flags */
    else
        unknown40 = flags | 2;
}

namespace M_LoadEmitmeshFromBrx__FP8EMITMESHP18CBinaryInputStream {
typedef int qword __attribute__((mode(TI)));
struct AL { int cb; int cbAlign; AL(int c, int a) { cb = c; cbAlign = a; } };
struct CBinaryInputStream;
struct TRI { int aipos[3]; float sArea; };
struct EMITMESH { int cpos; qword *apos; int ctri; TRI *atri; qword *anormal; float sArea; int pad18[2]; qword center; };
extern "C" {
int U16Read__18CBinaryInputStream(CBinaryInputStream *pbis);
void ReadVector__18CBinaryInputStreamP6VECTOR(CBinaryInputStream *pbis, qword *pv);
float F32Read__18CBinaryInputStream(CBinaryInputStream *pbis);
void *AllocFromWorldHeap(AL *pal);
void *AllocClearedFromWorldHeap(AL *pal);
}
extern "C" { extern qword D_002A2DC0; }
static inline void *PvAlloc(int cb, int cbAlign) { AL al(cb, cbAlign); return AllocFromWorldHeap(&al); }
static inline void *AllocZeroedFromHeap(int cb, int cbAlign) { AL al(cb, cbAlign); return AllocClearedFromWorldHeap(&al); }
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
// Reads an emitter mesh (particles spawn on its surface) from the level file: the vertex positions, then the
// triangles, accumulating each triangle's area into the mesh's total so emission can pick triangles in proportion to
// area; normals are zeroed for later computation.
extern "C" void LoadEmitmeshFromBrx__FP8EMITMESHP18CBinaryInputStream(EMITMESH *pemitmesh, CBinaryInputStream *pbis)
{
    int i;
    pemitmesh->cpos = U16Read__18CBinaryInputStream(pbis);
    pemitmesh->apos = (qword *)PvAlloc(pemitmesh->cpos * 16, 16);
    pemitmesh->anormal = (qword *)AllocZeroedFromHeap(pemitmesh->cpos * 16, 16);
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
        qword normal = VuCross(VuSub(apos[ptri->aipos[1]], p0), VuSub(apos[ptri->aipos[2]], p0));
        for (i = 0; i < 3; i++) {
            qword *pn = &pemitmesh->anormal[ptri->aipos[i]];
            *pn = VuAdd(*pn, normal);
        }
    }
    ReadVector__18CBinaryInputStreamP6VECTOR(pbis, &pemitmesh->center);
    for (i = 0; i < pemitmesh->cpos; i++)
        VNormalize(&pemitmesh->anormal[i], pemitmesh->anormal[i], D_002A2DC0);
}
} // namespace M_LoadEmitmeshFromBrx__FP8EMITMESHP18CBinaryInputStream

// Forwards to 0x1C4848(&g_sw, p) (SW-level registration of the emitter) (inferred)
extern "C" void AddEmitterToHeroFollowers(EMITTERV *pemitter)
{
    AddSwPoFollower(&D_003DD490, pemitter);
}

namespace M_PostEmitterLoad__FP7EMITTER { extern "C" {
struct K { short a; short b; int c; };
struct LM { float minValue; float maxValue; };
struct BOX { float xMin, yMin, zMin, pad; float xMax, yMax, zMax; };
struct EMITO { void *palo; };
struct EMITBV {
    int cref;                   /* +0x00 share count */
    int pad4;
    struct EMITO *pemito;       /* +0x08 the origin record (its first word: the object emitted from) */
    int padC;
    int originKind;             /* +0x10 EMITOK: 1 = box */
    char pad14[0x20 - 0x14];
    int originOid;              /* +0x20 */
    char pad24[0x30 - 0x24];
    struct BOX *pbox;           /* +0x30 the origin box (box kind) */
};
struct VTE { short delta; short pad; void (*pfn)(void *, void *); };
/* EMITTER (field meanings from Sly 1's EMITTER, same members in the same order) */
struct EMITTERV {
    char pad0[0xC]; struct VTE *pvt; char pad10[0x40 - 0x10];
    int flags40;                /* +0x40 LO flags, or (bit 0 clear) a pointer to them */
    char pad44[0xE0 - 0x44];
    int isDirty;                /* +0xE0 */
    struct EMITBV *pemitb;      /* +0xE4 */
    char padE8[0xF0 - 0xE8];
    int emitMode;               /* +0xF0 EMITRK */
    int particleCount;          /* +0xF4 */
    struct LM rate;             /* +0xF8 particles per second (burst modes: per burst) */
    int isCountDensity;         /* +0x100 scale the rate by the origin box (volume for constant count, else area) */
    float pauseChance;          /* +0x104 probability of a pause after a burst */
    struct LM pauseTime;        /* +0x108 pause duration range */
    float constantCount;        /* +0x110 particles to keep alive (constant-count mode) */
    int autoPause;              /* +0x114 */
    int groupOid;               /* +0x118 */
    char pad11C[0x130 - 0x11C]; /* +0x11C group list, +0x128 group link */
    float currentRate;          /* +0x130 rate drawn from `rate` */
    float rateRedrawInterval;   /* +0x134 */
    float rateRedrawTime;       /* +0x138 */
    float density;              /* +0x13C rate multiplier (box volume or area when isCountDensity) */
    float boxRadius;            /* +0x140 half the largest box extent */
};
struct ALLOCREQ { int cb; int kind; };
extern char D_003DD490[];
struct LONGU { long l; } __attribute__((packed));
extern const struct LONGU D_002A5140;
extern float D_002A5148;
void PostSwCallback(void *pg, struct EMITTERV *p, long k, int n);
float RandomFloatInRange__Fff(float gMin, float gMax);
void PauseEmitterIndefinite__FP7EMITTER(struct EMITTERV *p);
void PauseEmitter__FP7EMITTERf(struct EMITTERV *p, float g);
struct EMITTERV *FindSwObject__FP2SWi3OIDP2LO(void *pg, int kind, int oid, int n);
struct EMITTERV *CreateLo__F3CIDP2SO3OIDi(int cid, void *p, int oid, int n);
void *AllocCopyFromWorldHeap(struct ALLOCREQ *preq, void *pv);
void SetLoBusyKind(struct EMITTERV *p, int n);
void *GetLoWorldMatrix(struct EMITTERV *p);
void AppendDlEntry__FP2DLPv(void *pv, struct EMITTERV *p);

static __inline__ void *AllocCopyFromHeap(int cb, void *pv)
{
    struct ALLOCREQ req;
    req.cb = cb;
    req.kind = 0x10;
    return AllocCopyFromWorldHeap(&req, pv);
}


// Post-load setup of an EMITTER: runs the shared emitter template, schedules a callback when it emits from an object,
// infers the emission mode when the level left it unset (intermittent when it can pause, constant count when it has
// one, else continuous), draws the first rate and pause, computes the box density / radius, and when the emitter
// belongs to a group (groupOid) links it into the group emitter, creating that emitter from this one's settings if
// the level has none.
void PostEmitterLoad__FP7EMITTER(struct EMITTERV *pemitter)
{
    ::func_002586D8((::EMITTER *)pemitter);   /* sly2/emitter.h template (the EMITTER instance) */
    if (pemitter->pemitb->pemito->palo)
        PostSwCallback(D_003DD490, pemitter, D_002A5140.l, 0);
    if (pemitter->emitMode == EMIT_MODE_UNSET) {
        if (pemitter->pauseChance != 0.0f || pemitter->autoPause)
            pemitter->emitMode = EMIT_MODE_INTERMITTENT;
        else if (pemitter->constantCount != 0.0f)
            pemitter->emitMode = EMIT_MODE_CONSTANT_COUNT;
        else
            pemitter->emitMode = EMIT_MODE_CONTINUOUS;
    }
    switch (pemitter->emitMode) {
    case EMIT_MODE_BURST:
        pemitter->rateRedrawTime = D_002A5148;
        pemitter->currentRate = RandomFloatInRange__Fff(pemitter->rate.minValue, pemitter->rate.maxValue);
        if (pemitter->autoPause)
            PauseEmitterIndefinite__FP7EMITTER(pemitter);
        else if (RandomFloatInRange__Fff(0.0f, 1.0f) <= pemitter->pauseChance)
            PauseEmitter__FP7EMITTERf(pemitter, RandomFloatInRange__Fff(pemitter->pauseTime.minValue, pemitter->pauseTime.maxValue));
        break;
    case EMIT_MODE_INTERMITTENT:
        pemitter->rateRedrawTime = D_002A5148;
        pemitter->currentRate = RandomFloatInRange__Fff(pemitter->rate.minValue, pemitter->rate.maxValue) * 60.0f;
        if (pemitter->autoPause)
            PauseEmitterIndefinite__FP7EMITTER(pemitter);
        else if (RandomFloatInRange__Fff(0.0f, 1.0f) <= pemitter->pauseChance)
            PauseEmitter__FP7EMITTERf(pemitter, RandomFloatInRange__Fff(pemitter->pauseTime.minValue, pemitter->pauseTime.maxValue));
        break;
    case EMIT_MODE_CONTINUOUS:
    case EMIT_MODE_CONSTANT_COUNT:
        pemitter->rateRedrawInterval = pemitter->rateRedrawTime = 1.0f;
        pemitter->currentRate = RandomFloatInRange__Fff(pemitter->rate.minValue, pemitter->rate.maxValue);
        break;
    }
    pemitter->density = 1.0f;
    if (pemitter->pemitb->originKind == 1) {
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
            pemitter->boxRadius = g * 0.5f;
        }
        if (pemitter->isCountDensity) {
            if (pemitter->constantCount != 0.0f)
                pemitter->density = (pbox->xMax - pbox->xMin) * (pbox->yMax - pbox->yMin) * (pbox->zMax - pbox->zMin) * 1e-6f;
            else
                pemitter->density = (pbox->yMax - pbox->yMin) * (pbox->zMax - pbox->zMin) * 1e-4f;
        }
    }
    if (pemitter->groupOid != -1) {
        struct EMITTERV *group = FindSwObject__FP2SWi3OIDP2LO(D_003DD490 /* g_sw */, 5, pemitter->groupOid, 0);
        if (!group) {
            struct VTE *pvte;
            void *pthis;
            group = CreateLo__F3CIDP2SO3OIDi(0x1D /* CID_EMITTER */, 0, pemitter->groupOid, -1);
            group->pemitb = (__typeof__(group->pemitb))AllocCopyFromHeap(0x100, pemitter->pemitb);
            group->pemitb->cref = 1;
            if (pemitter->pemitb->pemito->palo) {
                group->pemitb->pemito = (__typeof__(group->pemitb->pemito))AllocCopyFromHeap(0x14, pemitter->pemitb->pemito);
                group->pemitb->originOid = -1;
                group->pemitb->pemito->palo = 0;
            }
            group->emitMode = pemitter->emitMode;
            group->particleCount = pemitter->particleCount;
            group->rate = pemitter->rate;
            group->isCountDensity = pemitter->isCountDensity;
            group->pauseChance = pemitter->pauseChance;
            group->pauseTime = pemitter->pauseTime;
            group->constantCount = pemitter->constantCount;
            group->autoPause = pemitter->autoPause;
            SetLoBusyKind(group, 1);
            pvte = &group->pvt[30];     /* place it at this emitter's transform (func_001A6318) */
            pthis = (char *)group + pvte->delta;
            pvte->pfn(pthis, GetLoWorldMatrix(pemitter));
        }
        pemitter->isDirty = 0;
        AppendDlEntry__FP2DLPv((char *)group + 0x11C /* group list */, pemitter);
        if (IsPointer(pemitter->flags40))
            *(int *)pemitter->flags40 &= ~2;
        else
            pemitter->flags40 &= ~2;
    }
}
} } // namespace M_PostEmitterLoad__FP7EMITTER

// Re-parents the emitter to the LO found via +0xE4 (0x1A58D0(p, 0x1A5BF0(oid), 0)) (inferred)
extern "C" void AttachEmitterToReferencedObject(EMITTERV *pemitter)
{
    SetLoParent(pemitter, GetSoFromLo((int)*pemitter->pemitb->aploRef), 0);
}

extern "C" { void UnsubscribeLoObject(void *pv, void *po); }
// Emitter message handler: on message 1 (an object is going away) forgets that object if it is the emitter's rip
// or blip group (it subscribes to both).
void EMITTER::HandleMessage(MSGID msgid, void *pv)
{
    if (msgid == 1) {
        if (pv == pripg) {
            UnsubscribeLoObject(pv, this);
            pripg = 0;
        } else if (pv == pblipg) {
            UnsubscribeLoObject(pv, this);
            pblipg = 0;
        }
    }
}

#ifndef SLY2_ENUM_ENSK
#define SLY2_ENUM_ENSK
enum ENSK { ENSK_Nil = -1, ENSK_Get = 0, ENSK_Set = 1 };
#endif
EMITTERV * EnsureEmitter(EMITTERV *pemitter, ENSK ensk) __asm__("EnsureEmitter__FP7EMITTER4ENSK");
// Accessor used by the property system: returns the emitter, marking it dirty (values changed) when the access is a
// write.
EMITTERV *EnsureEmitter(EMITTERV *pemitter, ENSK ensk)
{
    if (ensk == ENSK_Set) {
        pemitter->fDirty = ensk;
    }
    return pemitter;
}

namespace M_ModifyEmitterParticles__FP7EMITTER {
typedef int qword __attribute__((mode(TI)));
union V { qword q; struct { float x, y, z, w; } v; float a[4]; };
struct BLK { qword apos[2][0x84]; int ibuf; int numBlips; int pad[2]; BLK *pblkNext; };   /* a block of blips, double-buffered positions */
struct BLIPG { char pad[0x448]; BLK *pblkFirst; };
struct ALO;
struct EMITO { ALO *referenceObject; };
struct EMITBV { int pad0; int particleKind; EMITO *pemito; };
struct EMITTERV { char pad[0xE4]; EMITBV *pemitb; char pad1[0x140 - 0xE8]; float boxRadius; char pad2[0x150 - 0x144]; BLIPG *pblipg; };
extern "C" {
qword GetLoWorldPos(ALO *referenceObject);
float ModPositive__Fff(float g, float gMod);
}
static inline void Wrap(float *pg, float &gMin, float &gMax, float &gBox, float *pdg)
{
    if (*pg < gMin || gMax < *pg) {
        *pdg = gMin + ModPositive__Fff(*pg - gMin, gBox) - *pg;
        *pg += *pdg;
    }
}
// Applies the emitter's box modifier to its live particles (when boxRadius is set): blips that leave the box of half-size
// boxRadius around the reference object (else the origin) wrap around to the opposite side, so a field of
// particles (snow, rain, dust) follows its object without visible edges. Rips are left alone.
extern "C" void ModifyEmitterParticles__FP7EMITTER(EMITTERV *pemitter)
{
    float boxRadius = pemitter->boxRadius;
    if (boxRadius > 0.0001f) {
        V vecBox, posMin, posMax, dpos;
        volatile int unused;
        vecBox.v.z = boxRadius; vecBox.v.y = boxRadius; vecBox.v.x = boxRadius;
        if (pemitter->pemitb->pemito->referenceObject) {
            posMin.q = VuSub(GetLoWorldPos(pemitter->pemitb->pemito->referenceObject), vecBox.q);
            posMax.q = VuAdd(GetLoWorldPos(pemitter->pemitb->pemito->referenceObject), vecBox.q);
        } else {
            qword m, k;
            __asm__("qmtc2.ni %1, %0" : "=j"(k) : "r"(-1.0f));
            dpos.q = k;
            __asm__("vmulx.xyzw %0, %1, %2x" : "=j"(m) : "j"(vecBox.q), "j"(k));
            posMin.q = m;
            posMax.q = vecBox.q;
        }
        float boxSize = pemitter->boxRadius + pemitter->boxRadius;
        vecBox.v.z = boxSize; vecBox.v.y = boxSize; vecBox.v.x = boxSize;
        if (pemitter->pemitb->particleKind == 0 && pemitter->pblipg) {
            for (BLK *pblk = pemitter->pblipg->pblkFirst; pblk; pblk = pblk->pblkNext) {
                qword *apos = pblk->apos[pblk->ibuf];
                for (int i = 0; i < pblk->numBlips; i++) {
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
extern "C" { extern CLOCK D_002DDED0; }
struct CM { char pad[0x1C]; float lodScale; char pad2[0x90 - 0x20]; qword pos; };   /* lodScale (inferred): scales
                                                                                        an object's busy radius */
extern "C" { extern CM D_002DDF40; }   /* g_cm, the main camera */
struct EMITO { void *referenceObject; void *shapeObject; };   /* the EMITB's resolved objects */
struct EMITBV { int pad0; int particleKind; EMITO *pemito; };   /* particleKind: 0 blips, 1 rips */
struct RIPG { char pad[0x15C]; int ripList; };
struct BLIPG { char pad[0x444]; int numBlips; };
/* EMITTER (see PostEmitterLoad's view) */
struct EMITTERV {
    char pad0[0x10];
    int psoParent;              /* +0x10 LO */
    char pad1[0x38 - 0x14];
    float sBusy;                /* +0x38 LO: the radius within which the object counts as near the camera */
    char pad2[0xE0 - 0x3C];
    int isDirty;                /* +0xE0 */
    EMITBV *pemitb;             /* +0xE4 */
    char padE8[8];
    int emitMode;               /* +0xF0 */
    int particleCount;          /* +0xF4 */
    float rateMin, rateMax;     /* +0xF8 */
    char pad3[0x104 - 0x100];
    float pauseChance;          /* +0x104 */
    float pauseTimeMin, pauseTimeMax;   /* +0x108 */
    float constantCount;        /* +0x110 */
    int autoPause;              /* +0x114 */
    int groupOid;               /* +0x118 */
    int groupList;              /* +0x11C DL head: the first slaved emitter */
    char pad4[0x128 - 0x120];
    EMITTERV *nextInGroup;      /* +0x128 */
    char pad5[0x130 - 0x12C];
    float currentRate;          /* +0x130 */
    float rateRedrawInterval;   /* +0x134 */
    float rateRedrawTime;       /* +0x138 */
    float density;              /* +0x13C */
    char pad6[0x144 - 0x140];
    float particleFraction;     /* +0x144 fractional particle carried to the next frame */
    char pad7[0x14C - 0x148];
    RIPG *pripg;                /* +0x14C */
    BLIPG *pblipg;              /* +0x150 */
};
struct EMITX { BLIPG **ppblipg; RIPG **ppripg; EMITTERV *pemitter; };   /* EMITG */
extern "C" {
void UpdateLo(EMITTERV *p);
int IsDlEmpty__FP2DL(int *pdl);
int IsLoInWorld(EMITTERV *p);
qword GetLoWorldPos(void *p);
float RandomFloatInRange__Fff(float gMin, float gMax);
int CountDlEntries__FP2DL(int *pdl);
void *func_002333B4(void *pv, int b, int cb) __asm__("memset");
void EmitParticles__FiP5EMITBP5EMITG(int c, EMITBV *pemitb, EMITX *px);
}
void OnEmitterValuesChanged(EMITTERV *p) __asm__("OnEmitterValuesChanged__FP7EMITTER");
int IsEmitterPaused(EMITTERV *p) __asm__("IsEmitterPaused__FP7EMITTER");
void PauseEmitter(EMITTERV *p, float dt) __asm__("PauseEmitter__FP7EMITTERf");
void PauseEmitterIndefinite(EMITTERV *p) __asm__("PauseEmitterIndefinite__FP7EMITTER");
void ModifyEmitterParticles(EMITTERV *p) __asm__("ModifyEmitterParticles__FP7EMITTER");

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

// Per-frame emitter update: applies changed values and updates the LO; a slaved emitter (groupOid set) stops there.
// Otherwise it emits only when near the camera (a group: any active member within its busy radius) and not paused;
// the mode decides how many particles this frame (see EMIT_MODE), which go out through EmitParticles with the
// emitter as the fallback origin. Finally it redraws the rate when due and wraps particles into the box modifier.
extern "C" void UpdateEmitter__FP7EMITTERf(EMITTERV *emitter, float dt)
{
    if (emitter->isDirty)
        OnEmitterValuesChanged(emitter);
    UpdateLo(emitter);
    if (emitter->groupOid != -1)
        return;
    if (!IsDlEmpty__FP2DL(&emitter->groupList)) {
        EMITTERV *member;
        for (member = (EMITTERV *)emitter->groupList; member; member = member->nextInGroup) {
            if (!IsLoInWorld(member))
                continue;
            if (!member->psoParent)
                goto LActive;
            float radius = member->sBusy * D_002DDF40.lodScale;
            qword pos = GetLoWorldPos(member);
            radius = radius * radius;
            if (FLess(Dist2(pos, D_002DDF40.pos), radius))
                goto LActive;
        }
        return;
    } else if (emitter->psoParent) {
        float radius = emitter->sBusy * D_002DDF40.lodScale;
        qword pos = GetLoWorldPos(emitter);
        radius = radius * radius;
        if (FLess(radius, Dist2(pos, D_002DDF40.pos)))
            return;
    }
LActive:   /* near the camera (or not tied to a parent) */
    if (IsEmitterPaused(emitter))
        return;

    int numToEmit = 0;
    switch (emitter->emitMode) {
    case EMIT_MODE_INTERMITTENT: {
        float accumulated = emitter->particleFraction + emitter->currentRate * emitter->density * D_002DDED0.deltaTime;
        numToEmit = (int)accumulated;
        emitter->particleFraction = accumulated - (float)numToEmit;
        if (1.0f < (float)numToEmit) {
            int remaining = emitter->particleCount;
            if (remaining >= 0) {
                if (remaining <= numToEmit)
                    numToEmit = remaining;
                emitter->particleCount = remaining - numToEmit;
            }
            if (emitter->autoPause) {
                PauseEmitterIndefinite(emitter);
                emitter->currentRate = RandomFloatInRange__Fff(emitter->rateMin, emitter->rateMax) * 60.0f;
            } else if (RandomFloatInRange__Fff(0.0f, 1.0f) <= emitter->pauseChance) {
                PauseEmitter(emitter, RandomFloatInRange__Fff(emitter->pauseTimeMin, emitter->pauseTimeMax));
                emitter->currentRate = RandomFloatInRange__Fff(emitter->rateMin, emitter->rateMax) * 60.0f;
            }
        }
        break;
    }
    case EMIT_MODE_BURST: {
        numToEmit = (int)emitter->currentRate;
        int remaining = emitter->particleCount;
        if (remaining >= 0) {
            if (remaining <= numToEmit)
                numToEmit = remaining;
            emitter->particleCount = remaining - numToEmit;
        }
        if (emitter->autoPause)
            PauseEmitterIndefinite(emitter);
        else if (RandomFloatInRange__Fff(0.0f, 1.0f) <= emitter->pauseChance)
            PauseEmitter(emitter, RandomFloatInRange__Fff(emitter->pauseTimeMin, emitter->pauseTimeMax));
        emitter->currentRate = RandomFloatInRange__Fff(emitter->rateMin, emitter->rateMax);
        break;
    }
    case EMIT_MODE_CONTINUOUS: {
        float accumulated = emitter->particleFraction + emitter->currentRate * emitter->density * D_002DDED0.deltaTime;
        numToEmit = (int)accumulated;
        emitter->particleFraction = accumulated - (float)numToEmit;
        if (numToEmit != 0) {
            int remaining = emitter->particleCount;
            if (remaining >= 0) {
                if (remaining <= numToEmit)
                    numToEmit = remaining;
                emitter->particleCount = remaining - numToEmit;
            }
        }
        break;
    }
    case EMIT_MODE_CONSTANT_COUNT: {
        int numWanted = (int)(emitter->constantCount * emitter->density);
        int numAlive;
        if (!emitter->pemitb->particleKind)
            numAlive = emitter->pblipg ? emitter->pblipg->numBlips : 0;
        else
            numAlive = emitter->pripg ? CountDlEntries__FP2DL(&emitter->pripg->ripList) : 0;
        int shortfall = numWanted - numAlive;
        numToEmit = 0;
        int hasShortfall = 0 < shortfall;   /* a flag temp: `if (shortfall > 0)` compiles differently */
        if (hasShortfall)
            numToEmit = shortfall;
        break;
    }
    }

    if (numToEmit) {
        int unused[4];
        EMITX emitg;
        func_002333B4(&emitg, 0, sizeof(emitg));
        emitg.pemitter = emitter;
        emitg.ppblipg = &emitter->pblipg;
        emitg.ppripg = &emitter->pripg;
        EMITO *pemito = emitter->pemitb->pemito;
        void *savedReference = pemito->referenceObject;
        if (!savedReference && !pemito->shapeObject)
            pemito->referenceObject = emitter;
        EmitParticles__FiP5EMITBP5EMITG(numToEmit, emitter->pemitb, &emitg);
        emitter->pemitb->pemito->referenceObject = savedReference;
    }

    if (D_002DDED0.time >= emitter->rateRedrawTime) {
        emitter->currentRate = RandomFloatInRange__Fff(emitter->rateMin, emitter->rateMax);
        emitter->rateRedrawTime = D_002DDED0.time + emitter->rateRedrawInterval;
    }
    ModifyEmitterParticles(emitter);
}
} // namespace M_UpdateEmitter__FP7EMITTERf

void PauseEmitter(EMITTERV *pemitter, float dt) __asm__("PauseEmitter__FP7EMITTERf");
// Pauses the emitter for dt seconds (until the clock passes tUnpause).
void PauseEmitter(EMITTERV *pemitter, float dt)
{
    pemitter->tUnpause = D_002DDED0.time + dt;
}

void PauseEmitterIndefinite(EMITTERV *pemitter) __asm__("PauseEmitterIndefinite__FP7EMITTER");
// Pauses the emitter until it is explicitly unpaused (tUnpause = the largest float).
void PauseEmitterIndefinite(EMITTERV *pemitter)
{
    pemitter->tUnpause = D_002A514C;
}

void UnpauseEmitter(EMITTERV *pemitter) __asm__("UnpauseEmitter__FP7EMITTER");
// Unpauses the emitter now.
void UnpauseEmitter(EMITTERV *pemitter)
{
    pemitter->tUnpause = -1.0f;
}

// Returns whether the emitter is paused: before its unpause time, or (continuous and constant-count modes) with no particles left to
// emit.
int IsEmitterPaused(EMITTER *pemitter) __asm__("IsEmitterPaused__FP7EMITTER");
int IsEmitterPaused(EMITTER *pemitter)
{
    if (D_002DDED0.time < pemitter->tUnpause)
        return 1;
    switch (pemitter->emitMode) {
    case EMIT_MODE_CONTINUOUS:
    case EMIT_MODE_CONSTANT_COUNT:
        if (pemitter->particleCount == 0)
            return 1;
    }
    return 0;
}

void OnEmitterValuesChanged(EMITTERV *pemitter) __asm__("OnEmitterValuesChanged__FP7EMITTER");
// Called when the emitter's properties changed: clears the dirty flag, pushes the new emission parameters to its blip
// and rip groups and restarts the emission timer from the rate range (per minute in mode 3).
void OnEmitterValuesChanged(EMITTERV *pemitter)
{
    pemitter->fDirty = 0;
    if (pemitter->pblipg != 0) {
        SetBlipgEmitb(pemitter->pblipg, pemitter->pemitb);
    }
    if (pemitter->pripg != 0) {
        SetRipgEmitb(pemitter->pripg, pemitter->pemitb);
    }
    if (pemitter->emitMode == 3) {
        pemitter->currentRate = RandomFloatInRange(pemitter->rate.minValue, pemitter->rate.maxValue) * 60.0f;
    } else {
        pemitter->currentRate = RandomFloatInRange(pemitter->rate.minValue, pemitter->rate.maxValue);
    }
}

// Sets the emitter's particle count; in continuous mode, 0 pauses it and -1 unpauses it.
void SetEmitterParticleCount(EMITTER *pemitter, int c) __asm__("SetEmitterParticleCount__FP7EMITTERi");
void SetEmitterParticleCount(EMITTER *pemitter, int c)
{
    switch (pemitter->emitMode) {
    case EMIT_MODE_CONTINUOUS:
        if (c == 0)
            PauseEmitterIndefinite(pemitter);
        else if (c == -1)
            UnpauseEmitter(pemitter);
        else
            pemitter->particleCount = c;
        break;
    case EMIT_MODE_UNSET:
        pemitter->particleCount = c;
        break;
    }
    pemitter->fDirty = 1;
}

void SetEmitterAutoPause(EMITTERV *pemitter, int fAutoPause) __asm__("SetEmitterAutoPause__FP7EMITTERi");
// Sets the emitter's auto-pause flag (the condition that pauses it is read elsewhere; inferred to be its source going
// out of view).
void SetEmitterAutoPause(EMITTERV *pemitter, int fAutoPause)
{
    pemitter->autoPause = fAutoPause;
    pemitter->fDirty = 1;
}

namespace M_WakeSoWater__FP2SOP5WATERfP6VECTORT3ff {
// WakeSoWater (0x15A2C0): wake sound, ripple and splash for an SO moving through WATER.
// (Lilly's agent, T1683) MATCH, no register pins. Built from the lilly1/v3 drafts (Claude agents, T1679 / lane E20).
//
// Sly 2 signature (SO *, WATER *, qword pos, qword v, float dt, float gFrac, float gScale) despite the Sly 1 mangled name.
// The splash is EmitParticlesAt(pos, dir, cParticle, EMITP, EMITV, &e, 0, 0) with the EMITP (0x40) and EMITV (0x70) blocks
// passed BY VALUE (EABI: a3 / t0 point at the caller's copies). They are aggregate initializers: g++ builds each one
// in a memset stack temp and block-copies it, which is the target's memset + stores + lq/sq copy. What matched is the
// unions in the two types (as in StaticInit_emitter): store_constructor emits a (clobber (mem:BLK)) for every union
// initializer, and those clobbers are scheduler barriers.
// - EMITV: G4V at +0x00, a 4-float union at +0x40 and Sly 1's CLQ at +0x60 give the target's FPU numbers for
//   5.0 / 0.39 / 1.047 / -1 (-1 reuses 0.39's register) and the store order.
// - EMITP: a CLQ at +0x18 ({8, 12, 1}; Sly 1's EMITRIP has a clqScale) puts the +0x10 store before the +0xC store.
//   Any union holding +0x18/+0x1C matches; field names other than the Sly 1 types are unknown.
// - E is 0x14 bytes (memset sizeof); the splash position (v raised 30 in z) is its own stack local.
typedef int qword __attribute__((mode(TI)));

// Sly 1 util.h CLQ: quadratic g0 + x*(g1 + x*g2).
union CLQ {
    struct { float g0, g1, g2, unused; };
    float coefficients[4];
};

// 16-byte float views; the field names are unknown (same type as in StaticInit_emitter).
union G4V { struct { float g0, g4, g8, gC; }; float ag[4]; };

/* the particle record (EMITB+0xC0, emitter.h's EMITV) as rips use it (Sly 1's EMITRIP, objects as OIDs) */
struct EMITP {
    int ripKind;            /* +0x00 RIPT of each particle */
    int ripGroupKind;       /* +0x04 kind of the RIPG they go into */
    int renderOid, nextRenderOid, touchOid;   /* +0x08 here: WATER's splash and ripple objects, and the WATER */
    int unknown14;
    CLQ scaleRange;         /* +0x18 g0/g1: initial scale range (EmitRips); g2 unknown */
    int unknown28, unknown2C;
    qword unknown30;        /* some 16-byte member: the target copies EMITP with lq/sq, so the type is 16-aligned */
};

/* the velocity record (see EMITDEF::EMITV) */
struct EMITV {
    G4V speed;              /* +0x00 speed range, damping, z speed ratio */
    qword addedVelocity;    /* +0x10 */
    qword curlAxis;         /* +0x20 */
    int curlKind;           /* +0x30 */
    int particlesPerRing;   /* +0x34 */
    float randomRadius, lifetime;   /* +0x38, +0x3C */
    G4V tilt;               /* +0x40 tilt range (radians) */
    int skipBirth[4];       /* +0x50 skip and birth time ranges */
    CLQ alpha;              /* +0x60 */
};

typedef char check_emitp[(sizeof(EMITP) == 0x40) ? 1 : -1];
typedef char check_emitv[(sizeof(EMITV) == 0x70) ? 1 : -1];

struct VTE { short d; short pad; qword (*pfn)(void *); };
struct VT { VTE a[64]; };
struct XE8 { char pad[0x80]; qword velocity; };   /* SO+0xE8: its motion state */
struct SO { int pad0[3]; VT *pvt; char pad10[0xE8 - 0x10]; XE8 *motion; };
struct SND { char pad[0x170]; float lastWakeTime; void *wakeSound; };   /* the SO as a sound-carrying object (CID 0x32) */
struct EVT { short d; short pad; void (*pfn)(void *, float, qword, int); };
struct EVTT { EVT a[4]; };
struct EX { char pad[0x18]; int oid; };   /* an LO */
struct WATER { char pad[0x18]; int oid; char pad1c[0x178 - 0x1C]; EX *splashObject; EX *rippleObject; int hasWakeSound; };
struct RIP { char pad[0x1C]; EX *renderObject; float scaleStart; float scaleSlope; char pad28[0xEC - 0x28]; EVTT *pvt; };   /* +0x20: the scale CLQ, start + slope * t */
struct E { int pad0[2]; EX *renderObject; EX *nextRenderObject; WATER *touchObject; };   /* the resolved objects (EMITB+0x08's block) */
union V4 { qword q; float a[4]; };

extern "C" {
extern CLOCK D_002DDED0;
extern int D_002BDEB0[];
extern qword D_002A2DC0;
extern char D_003DD490[];
float RandomFloatInRange__Fff(float lo, float hi);
int RandomIntInRange__Fii(int lo, int hi);
int IsBasicDerivedFrom__FP5BASIC3CID(SO *p, int n);
void SetSndVolume(void *p, float g);
void StopSnd(void *p, float g);
void *CreateSfxSound(int id);
void SetSndSourceLo(void *p, SO *pso);
void SetSndRange(void *p, float a, float b);
void RcPtr_Assign(void *pp, void *p);
RIP *NewRipInGroup(void *p, int k, int x);
void *func_002333B4(void *p, int c, int cb) __asm__("memset");
void EmitParticlesAt(qword pos, qword dir, int n, EMITP emitp, EMITV emitv, E *pe, int x, int y);
}

static inline qword Length(qword a)
{
    qword r, one;
    __asm__("vmul.xyz %0, %2, %2\n\tvaddw.x %1, $vf0, $vf0w\n\tvadday.x $ACC, %0, %0y\n\tvmaddz.x %0, %1, %0z\n\t"
            "vsqrt $Q, %0x\n\tvwaitq\n\tvaddq.x %0, $vf0, $Q"
        : "=j"(r), "=j"(one) : "j"(a));
    return r;
}

void WakeSoWater(SO *p, WATER *pwater, qword pos, qword v, float dt, float gFrac, float gScale) __asm__("WakeSoWater__FP2SOP5WATERfP6VECTORT3ff");
// Makes an object's wake in water: by distance from its last wake point and a random chance per dt, emits splash
// particles and plays the wake sound, scaled by speed (gFrac, gScale) (inferred from the body).
extern "C" void WakeSoWater__FP2SOP5WATERfP6VECTORT3ff(SO *so, WATER *water, qword pos, qword v, float dt, float minIntervalFrac, float scale)
{
    float volume = 0.0f;    /* 0 until the sound is set: also the clamp floor and StopSnd's argument */
    VTE *pvte = &so->pvt->a[33];
    float distance = QwordX(Length(VuSub(pos, pvte->pfn((char *)so + pvte->d))));
    int shouldWake = RandomFloatInRange__Fff(volume, 3.0f) < dt;
    SND *soundObject;
    float strength, interval;
    soundObject = 0;
    if (IsBasicDerivedFrom__FP5BASIC3CID(so, 0x32))
        soundObject = (SND *)so;
    if (distance > 10.0f || shouldWake) {
        interval = (distance - 10.0f) * 0.0012658228f;  /* 1/790: full strength at 800 units */
        strength = Clamp(interval, volume, 1.0f);
        if (soundObject && water->hasWakeSound) {
            void **soundSlot = &soundObject->wakeSound;
            volume = strength * 0.9f + 0.1f;
            if (soundObject->wakeSound) {
                SetSndVolume(soundObject->wakeSound, volume);
            } else {
                void *sound = CreateSfxSound(D_002BDEB0[RandomIntInRange__Fii(0, 3)]);
                SetSndSourceLo(sound, (SO *)soundObject);
                SetSndRange(sound, 100.0f, 1000.0f);
                SetSndVolume(sound, volume);
                RcPtr_Assign(soundSlot, sound);
            }
        }
        if (!shouldWake) {
            interval = 1.0f - strength * (1.0f - minIntervalFrac);
            if (soundObject) {
                shouldWake = interval < D_002DDED0.time - soundObject->lastWakeTime;
            } else {
                float now = D_002DDED0.time;
                int periodNow = (int)(now / interval);
                int periodNext = (int)((now + dt) / interval);
                shouldWake = periodNow != periodNext;
            }
        }
        if (shouldWake && water->rippleObject) {
            RIP *ripple = NewRipInGroup(D_003DD490 /* g_sw */, 7 /* ripple kind */, 0);
            if (ripple) {
                EVT *pvte = &ripple->pvt->a[1];
                pvte->pfn((char *)ripple + pvte->d, scale, v, 0);
                ripple->scaleStart = scale;
                ripple->scaleSlope = scale * 0.6f;
                ripple->renderObject = water->rippleObject;
                if (soundObject)
                    soundObject->lastWakeTime = D_002DDED0.time;
                if (500.0f < distance && water->splashObject) {
                    EMITP emitp = { 1, 0, water->splashObject->oid, water->rippleObject->oid, water->oid, 0, {{ 8.0f, 12.0f, 1.0f }} };
                    EMITV emitv = { {{ distance * 0.5f, distance * 0.75f, 0.0f, 1.0f }}, so->motion->velocity, D_002A2DC0, 0, 0, 1.0f, 5.0f,
                                    {{ 0.39269909f, 1.0471976f }}, { 0 }, {{ 1.0f, 0.0f, -1.0f }} };
                    E emitg;
                    V4 posSplash;
                    func_002333B4(&emitg, 0, sizeof(emitg));
                    posSplash.q = v;
                    posSplash.a[2] += 30.0f;
                    emitg.renderObject = water->splashObject;
                    emitg.nextRenderObject = water->rippleObject;
                    emitg.touchObject = water;
                    EmitParticlesAt(posSplash.q, D_002A2DC0, RandomIntInRange__Fii(4, 6), emitp, emitv, &emitg, 0, 0);
                }
            }
        }
    } else if (soundObject) {
        void **soundSlot = &soundObject->wakeSound;
        if (soundSlot && *soundSlot) {
            StopSnd(*soundSlot, volume);
            RcPtr_Assign(soundSlot, 0);
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
struct MESHO { char pad[0xE8]; int numLengths; int padEC; float *cumulativeLength; };   /* the curve shape */
struct EMITO { void *referenceObject; MESHO *shapeObject; };
struct LM { float minValue, maxValue; };
/* the origin record (EMITB+0x10) */
struct EP {
    int kind;               /* EMIT_ORIGIN */
    int normalKind;         /* +0x14 EMIT_NORMAL */
    int pad18[3];
    LM offset;              /* +0x24 random distance moved along the chosen direction */
    int pad2C;
    void *data;             /* +0x30 the point itself (point), BOXE (box) or EMITMESH (mesh) */
    int pad34[3];
    qword vector;           /* +0x40 direction, centre or tangent partner, by normalKind */
};
struct EMITBV {
    int pad0, pad4;
    EMITO *pemito;
    int padC;
    struct EP ep;
};
extern "C" {
float RandomFloatInRange__Fff(float gMin, float gMax);
void EvaluateShapeAtDistance(MESHO *pmesh, float g, qword *ppos, qword *pnormal);
MATRIX3 *GetLoWorldMatrix(void *palo);
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
// Chooses particle `index` of `count`'s origin position and direction from the EMITB's origin record: a point, a
// random point in a box, evenly spaced (jittered) along the curve shape, or on a random mesh triangle weighted by
// area; the direction per normalKind. Both are then normalized / offset: the position moves a random distance
// along the direction.
extern "C" void ChooseEmitoPos__FP5EMITOiiP6VECTORT3(EMITBV *emitb, int index, int count, qword *ppos, qword *pdir)
{
    EP *origin = &emitb->ep;
    LM *offset;
    {
    VU_VECTOR av[3];
    switch (emitb->ep.kind) {
    case EMIT_ORIGIN_POINT:
        *ppos = *(qword *)&origin->data;
        break;
    case EMIT_ORIGIN_BOX: {
        { VU_VECTOR avUnused[3]; } // unused 48-byte local: frees the slot an[] reuses in case 3
        BOXE *box = (BOXE *)origin->data;
        float *pf = (float *)ppos;
        pf[0] = RandomFloatInRange__Fff(box->xMin, box->xMax);
        pf[1] = RandomFloatInRange__Fff(box->yMin, box->yMax);
        pf[2] = RandomFloatInRange__Fff(box->zMin, box->zMax);
        break;
    }
    case EMIT_ORIGIN_CURVE: {
        MESHO *curve = emitb->pemito->shapeObject;
        if (!curve) {
            float *pf = (float *)ppos;
            pf[2] = 0.0f;
            pf[1] = 0.0f;
            pf[0] = 0.0f;
            return;
        }
        float segment = curve->cumulativeLength[curve->numLengths - 1] / (float)count;
        float s = (float)index * segment;
        float g = RandomFloatInRange__Fff(s, s + segment);
        switch (origin->normalKind) {
        default:
            EvaluateShapeAtDistance(curve, g, ppos, 0);
            break;
        case EMIT_NORMAL_CURVE_TANGENT:
            EvaluateShapeAtDistance(curve, g, ppos, pdir);
            goto normalize;
        case EMIT_NORMAL_CURVE_NORMAL: {
            EvaluateShapeAtDistance(curve, g, ppos, &av[0].q);
            *pdir = VuCross(av[0].q, emitb->ep.vector);
            goto normalize;
        }
        }
        break;
    }
    case EMIT_ORIGIN_MESH: {
        EMITMESH *mesh = (EMITMESH *)origin->data;
        float s = RandomFloatInRange__Fff(0.0f, 1.0f) * mesh->sArea;
        TRI *ptri;
        int itri;
        for (itri = 0; itri < mesh->ctri; itri++) {
            ptri = &mesh->atri[itri];
            s -= ptri->sArea;
            if (s <= 0.0f)
                break;
        }
        av[0].q = mesh->apos[ptri->aipos[0]];
        av[1].q = mesh->apos[ptri->aipos[1]];
        av[2].q = mesh->apos[ptri->aipos[2]];
        float u = RandomFloatInRange__Fff(0.0f, 1.0f);
        float v = RandomFloatInRange__Fff(0.0f, 1.0f - u);
        float w = 1.0f - u - v;
        *ppos = Combo3(u, av[0], v, av[1], w, av[2]);
        if (origin->normalKind == EMIT_NORMAL_MESH_NORMAL) {
            VU_VECTOR an[3];
            an[0].q = mesh->anormal[ptri->aipos[0]];
            an[1].q = mesh->anormal[ptri->aipos[1]];
            an[2].q = mesh->anormal[ptri->aipos[2]];
            *pdir = Combo3(u, an[0], v, an[1], w, an[2]);
            goto normalize;
        }
        break;
    }
    }

    switch (origin->normalKind) {
    case EMIT_NORMAL_VECTOR:
        *pdir = origin->vector;
        break;
    case EMIT_NORMAL_RADIAL:
        *pdir = VuSub(*ppos, origin->vector);
        break;
    case EMIT_NORMAL_SCREEN: {
        void *reference = emitb->pemito->referenceObject;
        if (reference) {
            MATRIX3 *pm = GetLoWorldMatrix(reference);
            *pdir = MulTMat(*pm, MulCam(&D_002DDF40, origin->vector));
        } else {
            *pdir = MulCam(&D_002DDF40, origin->vector);
        }
        break;
    }
    default:
        *pdir = D_002A2DA0;
        break;
    }
    }

normalize:
    offset = &emitb->ep.offset;
    {
        VU_VECTOR dir = Normalize(*pdir);
        *pdir = dir.q;
        qword pos = *ppos;
        *ppos = VuAdd(pos, Scale(VU_FLOAT(RandomFloatInRange__Fff(origin->offset.minValue, offset->maxValue)), dir));
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
struct Y { int pad0, pad4, velocityKind; };   /* Sly 1's EMITVK: 0 relative, 1 relative + spin, 2 no relative velocity, 3 world */
struct E { int pad0, pad4; struct SO **ppso; int padC; struct Y y; };   /* EMITB (ppso: the resolved objects, [0] the reference) */
void __ml__FRC7MATRIX4T0(struct M4 *pm, struct M4 *pa, struct BUF *pb);
qword ConvertMatPos(struct M4 *pm, int z, qword v);
qword ConvertMatVec(struct M4 *pm, int z, qword v);
qword GetXfVelocity(struct R *r, int z, qword v);

// Transforms a chosen particle position (and velocity, depending on mode) from the reference SO space to world space.
// (inferred)
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
    *pv = ConvertMatPos(&m, 0, *pv);
    if (py->velocityKind != EMIT_VELOCITY_WORLD) {
        *pw = ConvertMatVec(&m, 0, *pw);
        if (py->velocityKind != EMIT_VELOCITY_NO_RELATIVE)
            *pw = VuAdd(*pw, GetXfVelocity(r, 0, py->velocityKind == EMIT_VELOCITY_RELATIVE_SPIN ? *pv : r->m.r[3]));
    }
}
} } // namespace M_ConvertEmitoPosVec__FP5EMITOP6VECTORT1

namespace M_CalculateEmitvx__FiP2LMiP6EMITVX {
struct LM { float minValue; float maxValue; };
struct EMITVX { int c; float dg; float drad; float g; float rad; };
float NormalizeAngle(float rad) asm("NormalizeAngle__Ff");
float RandomFloatInRange(float gMin, float gMax) asm("RandomFloatInRange__Fff");
// Sets up an even distribution of emission directions: c directions around the circle (capped at cMax) times enough
// tilt steps to cover the LM tilt range, starting at a random pan angle.
extern "C" void CalculateEmitvx__FiP2LMiP6EMITVX(int c, LM *plm, int cMax, EMITVX *pemitvx)
{
    int n;
    float dg;
    if (c > 0)
        pemitvx->c = c < cMax ? c : cMax;
    else
        pemitvx->c = cMax;
    n = (cMax + (pemitvx->c - 1)) / pemitvx->c;
    dg = NormalizeAngle(plm->maxValue - plm->minValue);
    pemitvx->drad = 6.2831855f / pemitvx->c;
    pemitvx->dg = dg / n;
    pemitvx->g = plm->minValue + pemitvx->dg * 0.5f;
    pemitvx->rad = RandomFloatInRange(0.0f, 6.2831855f);
}
} // namespace M_CalculateEmitvx__FiP2LMiP6EMITVX

namespace M_ChooseEmitVelocity__FP6EMITVXffP2LMP6VECTORiT4 { extern "C" {
typedef int qword __attribute__((mode(TI)));
typedef union { qword q; float f[4]; } U;
typedef struct { qword r[3]; } M3;
struct EMITVX { int c; float dTilt; float dPan; float tiltBase; float panBase; };
struct LM { float minValue; float maxValue; };
extern qword D_002A2DA0;
extern qword D_002A2DB0;
float RandomFloatInRange__Fff(float a, float b);
float NormalizeAngle__Ff(float g);
qword SetVectorSphere(float pan, float tilt, float r);
void GetNormalVectors(qword a, qword b, qword c, M3 *pm);

// Picks the velocity of particle i from an EMITVX distribution: its pan/tilt grid cell, jittered by gRand, rotated
// into the emission normal's frame and scaled by speed sv.
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
        tilt += gRand * RandomFloatInRange__Fff(-0.5f, 0.5f) * pemitvx->dTilt;
        pan += gRand * RandomFloatInRange__Fff(-0.5f, 0.5f) * pemitvx->dPan;
    }
    tilt = NormalizeAngle__Ff(tilt + 1.5707964f);
    pan = NormalizeAngle__Ff(pan);
    v.q = SetVectorSphere(pan, tilt, RandomFloatInRange__Fff(plm->minValue, plm->maxValue));
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
/* the velocity record (EMITDEF::EMITV); passed on as the speed LM (its first two floats) */
struct P {
    char pad[0xC];
    float zSpeedRatio;          /* +0x0C */
    char pad1[0x38 - 0x10];
    float randomRadius;         /* +0x38 jitter of the direction grid, in cells */
    float lifetime;             /* +0x3C */
    char pad2[0x48 - 0x40];
    float skipMin, skipMax;     /* +0x48 seconds of motion applied at birth (pre-advanced particles) */
    float birthMin, birthMax;   /* +0x50 how long ago the particle was born */
};
extern struct CLOCK D_002DDED0;
void ChooseEmitVelocity__FP6EMITVXffP2LMP6VECTORiT4(void *pemitvx, float a, float b, struct P *plm, qword *pv, int n, qword *pvOut);
float RandomFloatInRange__Fff(float a, float b);

// Chooses particle `index`'s velocity (ChooseEmitVelocity over the speed range) and its birth / death times; a
// nonzero skip moves the position along the velocity, as if the particle had already been flying that long.
void ChooseEmitvVelocityAge__FP5EMITVP6EMITVXP5EMITOiP6VECTORN24PfT7(struct P *emitv, void *pemitvx, int unused, int index, qword *ppos, qword *pv, qword *pvel, float *ptBirth, float *ptDeath)
{
    float birthTime, skip;
    ChooseEmitVelocity__FP6EMITVXffP2LMP6VECTORiT4(pemitvx, emitv->randomRadius, emitv->zSpeedRatio, emitv, pv, index, pvel);
    birthTime = D_002DDED0.time;
    birthTime = birthTime - RandomFloatInRange__Fff(emitv->birthMin, emitv->birthMax);
    *ptBirth = birthTime;
    *ptDeath = birthTime + emitv->lifetime;
    skip = RandomFloatInRange__Fff(emitv->skipMin, emitv->skipMax);
    if (skip != 0.0f) {
        U u;
        qword r;
        {
            qword qwuOut;
            __asm__("qmtc2.ni %1, %0" : "=j"(qwuOut) : "r"(skip));
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
struct EMITBV;
/* the five objects an EMITB's OIDs resolve to (EMITB+0x08) */
struct EMITREFS { void *reference; int shape; int render; int nextRender; int touch; };
/* EMITB, the rip-related fields (the record at +0xC0 as rips use it; Sly 1's EMITRIP) */
struct EMITBV {
    char pad0[8];
    EMITREFS *pshd;                  /* +0x08 */
    char pad1[0xB0 - 0xC];
    qword alpha;                /* +0xB0 EMITV's alpha CLQ */
    char pad2[0xC0 - 0xC0];
    int ripKind;                /* +0xC0 */
    int ripGroupKind;           /* +0xC4 */
    char pad3[0xD8 - 0xC8];
    float scaleMin, scaleMax;   /* +0xD8 initial scale range */
    char pad4[4];
    float scaleSlope, scaleCurve;   /* +0xE4 scale CLQ coefficients (0, 0: keep the kind's) */
};
/* a RIP as it is filled at emission (rip.cpp's RIP) */
class RIPV {
public:
    char pad0[0x14];
    float birthTime, duration;  /* +0x14 */
    int renderObject;           /* +0x1C */
    char pad20[4];              /* +0x20 scale.g0, set by Set */
    float scaleSlope, scaleCurve;   /* +0x24 */
    char pad2C[4];
    qword alpha;                /* +0x30 */
    MATRIX3 orientation;        /* +0x40 */
    char pad70[0x20];
    qword velocity;             /* +0x90 */
    char emitdv[0xEC - 0xA0];   /* +0xA0 EMITDV (SetEmitdvEmitb) */
    virtual void Set(qword pos, float g, int n);
    virtual void Init(EMITBV *pemit);
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
struct EMITX { void *ppblipg; SO **ppripg; void *subscriber; };   /* EMITG */
extern "C" {
extern char D_003DD490[];
SO *NewRipGroup(void *p, int n);
void SetRipgEmitb__FP4RIPGP5EMITB(SO *pso, EMITBV *pemit);
void SubscribeLoObject(SO *pso, void *p);
RIPV *NewRipInGroup(void *p, int n, SO *pso);
float RandomFloatInRange__Fff(float a, float b);
void SetEmitdvEmitb__FP6EMITDVP5EMITB(void *p, EMITBV *pemit);
MATRIX3 *GetLoWorldMatrix(void *p);
int IsDlEmpty__FP2DL(void *p);
}
// Spawns `count` rips at the originated positions / velocities / times into the emitter's RIPG (creating and
// subscribing it on first use): each gets a random initial scale, the EMITB's scale curve, alpha curve, motion
// (SetEmitdvEmitb), render object and the reference object's orientation. A group left empty is told so (slot 9).
extern "C" void EmitRips__FP5EMITBP5EMITGiP6VECTORT3PfT5(EMITBV *emitb, EMITX *emitg, int count, qword *positions, qword *velocities, float *birthTimes, float *deathTimes)
{
    SO *ripGroup = 0;
    int i;
    RIPV *rip;
    if (emitg && emitg->ppripg)
        ripGroup = *emitg->ppripg;
    if (!ripGroup) {
        ripGroup = NewRipGroup(D_003DD490, emitb->ripGroupKind);
        if (!ripGroup)
            return;
        if (emitg && emitg->ppripg)
            *emitg->ppripg = ripGroup;
        SetRipgEmitb__FP4RIPGP5EMITB(ripGroup, emitb);
        if (emitg && emitg->subscriber)
            SubscribeLoObject(ripGroup, emitg->subscriber);
    }
    if (!ripGroup)
        return;
    for (i = 0; i < count && (rip = NewRipInGroup(D_003DD490, emitb->ripKind, ripGroup)) != 0; i++) {
        rip->Set(positions[i], RandomFloatInRange__Fff(emitb->scaleMin, emitb->scaleMax), emitb->pshd->touch);
        if (emitb->scaleSlope != 0.0f || emitb->scaleCurve != 0.0f) {
            rip->scaleSlope = emitb->scaleSlope;
            rip->scaleCurve = emitb->scaleCurve;
        }
        rip->velocity = velocities[i];
        rip->birthTime = birthTimes[i];
        rip->duration = deathTimes[i] - birthTimes[i];
        rip->alpha = emitb->alpha;
        SetEmitdvEmitb__FP6EMITDVP5EMITB(rip->emitdv, emitb);
        rip->renderObject = emitb->pshd->render;
        if (emitb->pshd->reference)
            rip->orientation = *GetLoWorldMatrix(emitb->pshd->reference);
        rip->Init(emitb);
    }
    if (IsDlEmpty__FP2DL((char *)ripGroup + 0x15C /* RIPG rip list */))
        ripGroup->V9();
}
} // namespace M_EmitRips__FP5EMITBP5EMITGiP6VECTORT3PfT5

namespace M_EmitBlips__FP5EMITBP5EMITGiP6VECTORT3PfT5T3T3 {
// MATCH (Claude agent, lane E21), from fresh3 (56). Fixes: (1) for-increment written `k++, i++` (loop.c prepends each biv to loop_iv_list, so the LAST-incremented biv's giv inits are emitted first in the preheader; the target computes i*16 before k*16/k*4, which clobbers v1 and stops reload inheriting k from the k<c test); (2) Dot's d output `=j` (not earlyclobber) so it shares a's vf2.
// DRAFT (Claude agent, lane E5): fresh rewrite, aligned 49 (old 80). Outer while(k<c && (pblk || (pblk=alloc))) gives the target loop shape; GRand(&p->lm) inline gives the hoisted plm slot; unsigned n0 &= ~1u gives lui/ori. Left: giv bases (target base = &apd[i] for a-side, separate &afr[1].apos[i] giv for the copy).
typedef int qword __attribute__((mode(TI)));
struct EMITTERV;
/* per-blip draw data (the third array of a frame) */
struct PD { unsigned int color; int frame; float rollSpeed; float rollAngle; };
struct PAIR { qword a; qword b; };
struct FR { qword apos[43]; qword av[43]; qword apd[43]; qword pad[3]; };
struct BLK {
    FR afr[2]; int unknown1080;
    int numBlips; PAIR *apair; int pad108C; BLK *pblkNext;   /* apair: spline end points (spline motion) */
};
struct PRG;
struct PRGVT { char pad[0x198]; short d; short pad2; void (*pfn)(PRG *, EMITTERV *); };
/* BLIPG (fields as the blip-emission code reads them; kinds after Sly 1's BLIPMK / BLIPOK) */
struct PRG {
    char pad0[0xC]; PRGVT *pvt; char pad10[0x160 - 0x10];
    int motionKind;             /* +0x160 2: spline (each blip keeps two end points) */
    char pad164[0x1A0 - 0x164];
    int numFrames;              /* +0x1A0 texture animation frames */
    char pad1A4[0x3B0 - 0x1A4];
    int numColors;              /* +0x3B0 */
    char pad3B4[0x434 - 0x3B4];
    int hasColorRanges;         /* +0x434 colours come in (start, end) pairs: pick an even index */
    int pad438;
    int renderKind;             /* +0x43C 2: rolling */
    int pad440;
    int numBlips;               /* +0x444 */
    BLK *pblkFirst;             /* +0x448 */
};
struct LM { float minValue, maxValue; };
/* EMITB, the blip-related fields (the record at +0xC0 as blips use it; Sly 1's EMITBLIP) */
struct EMITTERV {
    char pad[0xC8];
    int hasRandomFrame;         /* +0xC8 */
    char padCC[0xF0 - 0xCC];
    int rollKind;               /* +0xF0 0: mirror the roll for blips moving away from the camera */
    int hasRandomRoll;          /* +0xF4 random start angle */
    LM rollSpeed;               /* +0xF8 */
};
struct EMITX { PRG **ppprg; void *ppripg; EMITTERV *pemitter; };   /* EMITG (pemitter: the subscriber) */
struct CM { char pad[0x70]; qword dir; };
extern "C" {
extern CM D_002DDF40;
extern char D_003DD490[];
PRG *NewBlipGroup(void *pv);
void SetBlipgEmitb(PRG *pprg, EMITTERV *p);
BLK *NewBlip(void *pv, PRG *pprg);
void *func_002333B4(void *pv, int b, int cb) __asm__("memset");
int RandomIntInRange__Fii(int nMin, int nMax);
float RandomFloatInRange__Fff(float gMin, float gMax);
void EmitBlips__FP5EMITBP5EMITGiP6VECTORT3PfT5T3T3(EMITTERV *p, EMITX *px, int c, qword *apos, qword *av, float *ag1, float *ag2, qword *aa, qword *ab);
}

static inline float Dot(qword a, qword b)
{
    qword d, t;
    float g;
    __asm__("vmul.xyz %0, %2, %3\n\tvaddw.x %1, $vf0, $vf0w\n\tvadday.x $ACC, %0, %0y\n\tvmaddz.x %0, %1, %0z" : "=j"(d), "=&j"(t) : "j"(a), "j"(b));
    __asm__("qmfc2.ni %0, %1" : "=r"(g) : "j"(d));
    return g;
}

static inline float GRand(LM *plm) { return RandomFloatInRange__Fff(plm->minValue, plm->maxValue); }

// Spawns `c` blips into the emitter's BLIPG (creating it and handing it the subscriber on first use), filling the
// blocks' free slots: position + birth time, velocity + death time, and the draw data (random colour, start frame,
// roll speed / angle for rolling blips), plus the spline end points for spline motion; each slot is mirrored into the
// block's second frame buffer.
void EmitBlips__FP5EMITBP5EMITGiP6VECTORT3PfT5T3T3(EMITTERV *emitter, EMITX *px, int c, qword *apos, qword *av, float *ag1, float *ag2, qword *aa, qword *ab)
{
    PRG *pprg = 0;
    if (px && px->ppprg)
        pprg = *px->ppprg;
    if (!pprg) {
        pprg = NewBlipGroup(D_003DD490);
        if (!pprg)
            return;
        if (px && px->ppprg)
            *px->ppprg = pprg;
        SetBlipgEmitb(pprg, emitter);
        if (px && px->pemitter)
            pprg->pvt->pfn((PRG *)((char *)pprg + pprg->pvt->d), px->pemitter);
        if (!pprg)
            return;
    }
    BLK *pblk = pprg->pblkFirst;
    int k = 0;
    while (k < c && (pblk || (pblk = NewBlip(D_003DD490, pprg)))) {
        qword *pq1 = pblk->afr[1].apos;
        for (unsigned int i = pblk->numBlips; i < 43 && k < c; k++, i++) {
            FR *pfr = &pblk->afr[0];
            pfr->apos[i] = apos[k];
            ((float *)&pfr->apos[i])[3] = ag1[k];
            pfr->av[i] = av[k];
            ((float *)&pfr->av[i])[3] = ag2[k];
#define pd ((PD *)&pfr->apd[i])
            func_002333B4(pd, 0, sizeof(PD));
            pd->color = RandomIntInRange__Fii(0, pprg->numColors - 1);
            if (pprg->hasColorRanges)
                pd->color &= ~1u;
            if (emitter->hasRandomFrame)
                pd->frame = RandomIntInRange__Fii(0, pprg->numFrames - 1);
            if (pprg->renderKind == 2) {
                float g = GRand(&emitter->rollSpeed);
                pd->rollSpeed = g;
                if (!emitter->rollKind && 0.0f < Dot(D_002DDF40.dir, pfr->av[i]))
                    pd->rollSpeed = -g;
                if (emitter->hasRandomRoll)
                    pd->rollAngle = RandomFloatInRange__Fff(-3.14159265f, 3.14159265f);
            }
            if (pprg->motionKind == 2) {
                PAIR *ppair = &pblk->apair[i];
                ppair->a = aa[k];
                ppair->b = ab[k];
            }
            pq1[i] = pfr->apos[i];
            pq1[i + 43] = pfr->av[i];
            pq1[i + 86] = *(qword *)pd;
            pblk->numBlips++;
            pprg->numBlips++;
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
struct XF { qword pos; MATRIX3 localMatrix; qword v; qword w; };
struct EMITVX { int c; float dTilt; float dPan; float tiltBase; float panBase; };
/* EMITB: the origin's local transform, and EMITV's ring count and tilt range */
struct EMITTERV { char pad[0x1C]; XF *pxf; char pad1[0x84 - 0x20]; int particlesPerRing; char pad2[0x90 - 0x88]; char tilt[1]; };
struct BUF { qword *apos; qword *av; float *atBirth; float *atDeath; int convertToWorld; };   /* EMITGEN */
extern "C" {
void *AllocFromStack(AL *pal);
void CalculateEmitvx__FiP2LMiP6EMITVX(int n, void *p, int c, EMITVX *pvx);
void ChooseEmitoPos__FP5EMITOiiP6VECTORT3(EMITTERV *pemit, int i, int c, qword *ppos, qword *pnormal);
void ChooseEmitvVelocityAge__FP5EMITVP6EMITVXP5EMITOiP6VECTORN24PfT7(void *p, EMITVX *pvx, void *q, int n, qword *ppos, qword *pnormal, qword *pvel, float *ptBirth, float *ptDeath);
void ConvertEmitoPosVec__FP5EMITOP6VECTORT1(EMITTERV *pemit, qword *ppos, qword *pvel);
void OriginateParticles__FiP5EMITBP7EMITGEN(int c, EMITTERV *pemit, BUF *pb);
}
static inline void *PvAlloc(int cb, int cbAlign) { AL al(cb, cbAlign); return AllocFromStack(&al); }
static inline qword MulRow(const MATRIX3 &m, qword v)
{
    qword r;
    __asm__("vmulax.xyz ACC, %2, %1x\n\tvmadday.xyz ACC, %3, %1y\n\tvmaddz.xyz %0, %4, %1z" : "=j"(r) : "j"(v), "j"(m.r[0]), "j"(m.r[1]), "j"(m.r[2]));
    return r;
}
// Fills an EMITGEN with `c` new particles' positions, velocities and birth / death times: an even direction grid
// (CalculateEmitvx), each particle's origin (ChooseEmitoPos) and velocity / age (ChooseEmitvVelocityAge), moved by
// the origin's local transform (position, rotation, linear and angular velocity) and, when asked, into world space.
void OriginateParticles__FiP5EMITBP7EMITGEN(int c, EMITTERV *emitb, BUF *gen)
{
    gen->apos = (qword *)PvAlloc(c * 16, 16);
    gen->av = (qword *)PvAlloc(c * 16, 16);
    gen->atBirth = (float *)PvAlloc(c * 4, 16);
    gen->atDeath = (float *)PvAlloc(c * 4, 16);
    EMITVX vx;
    CalculateEmitvx__FiP2LMiP6EMITVX(emitb->particlesPerRing, emitb->tilt, c, &vx);
    for (int i = 0; i < c; i++) {
        union { qword q; float f[4]; } normal;
        ChooseEmitoPos__FP5EMITOiiP6VECTORT3(emitb, i, c, &gen->apos[i], &normal.q);
        ChooseEmitvVelocityAge__FP5EMITVP6EMITVXP5EMITOiP6VECTORN24PfT7((char *)emitb + 0x50, &vx, (char *)emitb + 0x10, i, &gen->apos[i], &normal.q, &gen->av[i], &gen->atBirth[i], &gen->atDeath[i]);
        XF *pxf = emitb->pxf;
        if (pxf) {
            {
                register qword *ppos __asm__("$2"); // register pin (target loads gen->apos into v0 and forms &apos[i] there)
                ppos = gen->apos;
                ppos[i] = VuAdd(MulRow(pxf->localMatrix, ppos[i]), pxf->pos);
            }
            qword d = VuSub(gen->apos[i], pxf->pos);
            qword x = VuCross(pxf->w, d);
            qword t = VuAdd(MulRow(pxf->localMatrix, gen->av[i]), pxf->v);
            gen->av[i] = VuAdd(t, x);
        }
        if (gen->convertToWorld)
            ConvertEmitoPosVec__FP5EMITOP6VECTORT1(emitb, &gen->apos[i], &gen->av[i]);
    }
}
} // namespace M_OriginateParticles__FiP5EMITBP7EMITGEN

namespace M_OriginateSplineSinkParticles {
// OriginateSplineSinkParticles: emitter particle transform (Claude agent, lane E20). The clock time is read into a local BEFORE
// the ptimes->at[i] load, so loop.c sees a 1-insn life for %hi(D_002DDED0) and doesn't hoist it.
typedef int qword __attribute__((mode(TI)));
struct M4 { qword r[4]; };
struct BUF { qword *apos; qword *av; float *atBirth; float *atDeath; int convertToWorld; };   /* EMITGEN */
struct SAV { int reference; };   /* EMITB+0x08's resolved objects: [0] */
struct EMITBV { char pad[8]; SAV *psav; char pad2[0x18 - 0xC]; int velocityKind; int localXf; };   /* velocityKind: Sly 1's EMITVK (2: positions only, 3: world) */
class ALO;
struct VT { char pad[0x100]; short d; short pad2; void *(*pfn)(ALO *, M4 *); };
class ALO { public: char pad[0xC]; VT *pvt; int psoParent; char pad2[0xE4 - 0x14]; EMITBV *pemitb; };   /* the spline target (an EXPLO) */
struct EO { char pad[8]; ALO *splineTarget; };
struct EMITTERV { char pad[8]; EO *peo; };
struct TIMES { char pad[0xC]; float *atDeath; };   /* the EMITGEN being emitted */
extern "C" {
extern CLOCK D_002DDED0;
extern float D_002A5150;
void OriginateParticles__FiP5EMITBP7EMITGEN(int c, EMITBV *pemitb, BUF *pb);
int AreFloatsNear__Ffff(float g1, float g2, float gEps);
void LoadIdentityMatrix4(void *p, int n, M4 *pm0, float t, M4 *pm1);
void __ml__FRC7MATRIX4T0(M4 *pmOut, M4 *pmA, M4 *pmB);
void OriginateSplineSinkParticles(int c, EMITTERV *pemit, TIMES *ptimes, BUF *pb);
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
static inline qword Scale(qword v, VU_FLOAT s) { qword d; __asm__("vmulx.xyzw %0, %1, %2x" : "=j"(d) : "j"(v), "j"(s.q)); return d; }

// For spline-motion blips: originates the spline end points from the spline target (an EXPLO with its own EMITB,
// whose reference object defaults to the target's parent), each moved by the target's motion at the particle's
// death time (LoadIdentityMatrix4 extrapolation) and with its velocity reversed, so blips fly into the target (inferred).
void OriginateSplineSinkParticles(int c, EMITTERV *emitb, TIMES *ptimes, BUF *gen)
{
    ALO *target = emitb->peo->splineTarget;
    SAV *refs = target->pemitb->psav;
    int saved = refs->reference;
    int i;
    float tPrev;
    void *pv;
    M4 m;
    M4 mA;
    M4 mB;
    M4 mBase;
    M4 mWorld;
    if (!saved)
        refs->reference = target->psoParent;
    gen->convertToWorld = 0;
    OriginateParticles__FiP5EMITBP7EMITGEN(c, target->pemitb, gen);
    target->pemitb->localXf = 0;
    tPrev = D_002A5150;
    pv = target->pvt->pfn((ALO *)((char *)target + target->pvt->d), &mWorld);
    mBase = mWorld;
    for (i = 0; i < c; i++) {
        float tNow = D_002DDED0.time;
        float t = ptimes->atDeath[i] - tNow;
        int velocityKind = target->pemitb->velocityKind;
        if (!AreFloatsNear__Ffff(t, tPrev, 0.0001f)) {
            M4 mT;
            tPrev = t;
            LoadIdentityMatrix4(pv, 0, &m, t, 0);
            __ml__FRC7MATRIX4T0(&mT, &m, &mBase);
            m = mT;
            if (velocityKind != EMIT_VELOCITY_WORLD) {
                LoadIdentityMatrix4(pv, 0, &mA, t, &mB);
                __ml__FRC7MATRIX4T0(&mT, &mA, &mWorld);
                mA = mT;
                __ml__FRC7MATRIX4T0(&mT, &mB, &mWorld);
                mB = mT;
            }
        }
        gen->apos[i] = MulAffine(m, gen->apos[i]);
        if (velocityKind != EMIT_VELOCITY_WORLD) {
            M4 *pmA = &mA;
            gen->av[i] = MulRot(*pmA, gen->av[i]);
            if (velocityKind != EMIT_VELOCITY_NO_RELATIVE) {
                qword v[1];
                if (velocityKind == EMIT_VELOCITY_RELATIVE_SPIN)
                    v[0] = gen->apos[i];
                else
                    v[0] = mA.r[3];
                gen->av[i] = VuAdd(gen->av[i], MulAffine(mB, v[0]));
            }
        }
        qword vv = gen->av[i];
        gen->av[i] = Scale(vv, VU_FLOAT(-1.0f));
    }
    target->pemitb->psav->reference = saved;
}
} // namespace M_OriginateSplineSinkParticles

namespace M_EmitParticles__FiP5EMITBP5EMITG {
struct LOC { int positions, velocities, birthTimes, deathTimes; int convertToWorld; int pad[3]; };   /* EMITGEN */
struct L2 { int splineStarts; int splineEnds; int pad[6]; };
struct B { int pad; int particleKind; char pad2[0xC4 - 8]; int blipMotionKind; };   /* EMITB: 1 rips; 2 spline motion */
extern "C" {
void OriginateParticles__FiP5EMITBP7EMITGEN(void *a, B *b, LOC *ploc);
void EmitRips__FP5EMITBP5EMITGiP6VECTORT3PfT5(B *b, void *c, void *a, int q0, int q1, int q2, int q3);
void OriginateSplineSinkParticles(void *a, B *b, LOC *ploc, L2 *pl2);
void EmitBlips__FP5EMITBP5EMITGiP6VECTORT3PfT5T3T3(B *b, void *c, void *a, int q0, int q1, int q2, int q3, int x, int y);
// Emits `count` particles from an EMITB: originates them (inside count stack-allocation scope), then hands them to
// EmitRips or EmitBlips (spline-motion blips also get their spline end points).
void EmitParticles__FiP5EMITBP5EMITG(void *count, B *emitb, void *emitg)
{
    if (count) {
        LOC gen;
        STACKSCOPE frame;
        gen.convertToWorld = 1;
        OriginateParticles__FiP5EMITBP7EMITGEN(count, emitb, &gen);
        if (emitb->particleKind == 1) {
            EmitRips__FP5EMITBP5EMITGiP6VECTORT3PfT5(emitb, emitg, count, gen.positions, gen.velocities, gen.birthTimes, gen.deathTimes);
        } else {
            L2 spline;
            if (emitb->blipMotionKind == 2) {
                OriginateSplineSinkParticles(count, emitb, &gen, &spline);
            } else {
                spline.splineStarts = 0;
                spline.splineEnds = 0;
            }
            EmitBlips__FP5EMITBP5EMITGiP6VECTORT3PfT5T3T3(emitb, emitg, count, gen.positions, gen.velocities, gen.birthTimes, gen.deathTimes, spline.splineStarts, spline.splineEnds);
        }
    }
}
}
} // namespace M_EmitParticles__FiP5EMITBP5EMITG

namespace M_EmitParticlesAt { extern "C" {
typedef int qword __attribute__((mode(TI)));
typedef struct M4 { qword aq[4]; } M4;
typedef struct B7 { qword aq[7]; } B7;
/* EMITB: the origin record at +0x10 (+0x30 point position, +0x40 direction), velocity at +0x50, particle at +0xC0 */
struct X { int cref; int particleKind; int refs; int pad; M4 origin; B7 velocity; M4 particle; };
struct Y { int ppblipg; int ppripg; int subscriber; };   /* EMITG */
void InitEmitBlock(struct X *px);
void EmitParticles__FiP5EMITBP5EMITG(void *p, struct X *px, struct Y *py);
void func_002333B4(void *pv, int b, int cb) __asm__("memset");
// Emits `count` rips from a temporary EMITB: the defaults (InitEmitBlock) with the given point origin and direction,
// particle and velocity records (passed by value by the callers), resolved objects (`refs`, EMITB+0x08) and rip
// group / subscriber.
void EmitParticlesAt(qword pos, qword direction, void *count, M4 *particle, B7 *velocity, int refs, int ppripg, int subscriber)
{
    struct X emitb;
    struct Y emitg;
    InitEmitBlock(&emitb);
    func_002333B4(&emitg, 0, sizeof(emitg));
    emitb.origin.aq[2] = pos;
    emitb.origin.aq[3] = direction;
    emitb.velocity = *velocity;
    emitb.refs = refs;
    emitg.ppripg = ppripg;
    emitg.subscriber = subscriber;
    emitb.particleKind = 1;     /* rips */
    emitb.particle = *particle;
    EmitParticles__FiP5EMITBP5EMITG(count, &emitb, &emitg);
}
} } // namespace M_EmitParticlesAt

// 0x15BF38: dead-code scrap (sly2/scrap.h)
SLY2_SCRAP("addiu $sp, $sp, 0x1b0");

namespace M_SetEmitdvEmitb__FP6EMITDVP5EMITB { extern "C" {
typedef int qword __attribute__((mode(TI)));
/* EMITB: EMITV's damping (+0x58), added velocity (+0x60), curl axis (+0x70) and curl rate (+0x80) */
struct S { char pad[0x58]; float damping; int pad5c; qword addedVelocity; qword curlAxis; float curlRate; };
struct O { qword addedVelocity; qword curlAxis; float curlRate; float damping; };   /* EMITDV: a particle's motion parameters */
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
// Copies an EMITB's particle motion parameters into an EMITDV (the curl axis normalized, +z if degenerate).
void SetEmitdvEmitb__FP6EMITDVP5EMITB(struct O *emitdv, struct S *emitb)
{
    emitdv->addedVelocity = emitb->addedVelocity;
    VNormalize(&emitdv->curlAxis, emitb->curlAxis, D_002A2DC0);
    emitdv->curlRate = emitb->curlRate;
    emitdv->damping = emitb->damping;
}
} } // namespace M_SetEmitdvEmitb__FP6EMITDVP5EMITB

namespace M_CalculateEmitdvMatrix__FP6EMITDVfP7MATRIX4 {
typedef int qword __attribute__((mode(TI)));
struct MATRIX4 { qword r[4]; };
struct MATRIX3 { qword r[3]; };
struct VU_FLOAT { qword q; VU_FLOAT(float f) { __asm__("" : "=r"(q) : "0"(*(int *)&f)); } };
extern "C" { extern CLOCK D_002DDED0; }
extern "C" { extern MATRIX4 D_002A2D30; }
struct X { float x, y, z, w; qword axis; float curlRate; float damping; };   /* EMITDV */
extern "C" { void BuildAxisAngleRotationMatrix(MATRIX3 *pm, float g, qword axis); }
MATRIX4 operator*(const MATRIX4 &a, const MATRIX4 &b) __asm__("__ml__FRC7MATRIX4T0");
MATRIX4 operator*(const MATRIX4 &m, VU_FLOAT s) __asm__("__ml__FRC7MATRIX4G8VU_FLOAT");
static inline MATRIX4 operator+(const MATRIX4 &a, const MATRIX4 &b) return r
{
    for (int i = 0; i < 4; i++)
        r.r[i] = VuAdd(a.r[i], b.r[i]);
}
extern "C" MATRIX4 CalculateEmitdvMatrix__FP6EMITDVfP7MATRIX4(X *p);
// Builds the matrix that advances a particle's velocity by one frame (dt): the curl rotation R (curlRate * dt about
// the axis), plus R * (damping * v + addedVelocity) * dt, as a 4x4 acting on (v, 1).
MATRIX4 CalculateEmitdvMatrix__FP6EMITDVfP7MATRIX4(X *emitdv)
{
    MATRIX4 mScale = D_002A2D30;
    float *pf = (float *)&mScale;
    pf[0] = emitdv->damping;
    pf[5] = emitdv->damping;
    pf[10] = emitdv->damping;
    pf[12] = emitdv->x;
    pf[13] = emitdv->y;
    pf[14] = emitdv->z;
    MATRIX4 mRot = D_002A2D30;
    MATRIX4 *pmRot = &mRot;
    if (emitdv->curlRate != 0.0f) {
        MATRIX3 m3;
        BuildAxisAngleRotationMatrix(&m3, emitdv->curlRate * D_002DDED0.deltaTime, emitdv->axis);
        *(MATRIX3 *)pmRot = m3;
    }
    MATRIX4 mD;
    mD = *pmRot * mScale * VU_FLOAT(D_002DDED0.deltaTime);
    return *pmRot + mD;
}
} // namespace M_CalculateEmitdvMatrix__FP6EMITDVfP7MATRIX4

namespace M_ExplodeWithDefaults { extern "C" {
typedef int qword __attribute__((mode(TI)));
struct VTE { short d; short pad; void (*pfn)(...); };
struct O { int a[3]; struct VTE *vt; };
struct P { int flags; int reference; float scale; float radius; qword vector; qword origin; qword rest; };   /* EXPLSO */
void func_002333B4(void *pv, int b, int cb) __asm__("memset");
// Explodes an EXPL with default (zero) parameters via its explode virtual (slot 51) (inferred)
void ExplodeWithDefaults(struct O *o)
{
    struct P p = {0};
    struct VTE *vt;

    vt = o->vt;
    vt[51].pfn((char *)o + vt[51].d, &p);
}
} } // namespace M_ExplodeWithDefaults

namespace M_ExplodeExplParams { extern "C" {
typedef int qword __attribute__((mode(TI)));
struct VTE { short d; short pad; void (*pfn)(...); };
struct O { int a[3]; struct VTE *vt; };
struct P { int flags; int reference; float scale; float radius; qword vector; qword origin; qword rest; };   /* EXPLSO */
// Explodes an EXPL with explicit flags/reference/vector/origin/scale/radius (packs an EXPLSO, calls slot 51)
// (inferred)
void ExplodeExplParams(struct O *o, int flags, int reference, float scale, float radius, qword vector, qword origin)
{
    struct P p;
    struct VTE *e;
    p.flags = flags;
    p.reference = reference;
    p.scale = scale;
    p.radius = radius;
    p.vector = vector;
    p.origin = origin;
    e = &o->vt[51];
    e->pfn((char *)o + e->d, &p);
}
} } // namespace M_ExplodeExplParams

// Empty base implementation of the EXPL explode virtual (slot 51) (inferred)
void EXPL::ExplodeExplso()   /* key function: EXPL's vtable comes out here (ExplodeExplExplso__FP4EXPLP6EXPLSO, empty) */
{
}

namespace M_LoadExplgFromBrx { extern "C" {
typedef struct MSG { int cb; int nKind; } MSG;
void *AllocClearedFromWorldHeap(MSG *pmsg);
static inline void *PvAllocMsg(MSG *pmsg, int cb, int nKind) { pmsg->cb = cb; pmsg->nKind = nKind; return AllocClearedFromWorldHeap(pmsg); }
struct C { int a[4]; };
struct VTE { short d; short pad; void (*pfn)(...); };
struct O { int a[3]; struct VTE *vt; int pad[6]; unsigned long fl; };
struct DA { int n0; int cMax; int c; struct O **a; };
struct S { char pad[0xE0]; int numChildren; struct O **children; };   /* EXPLG */
void STACKSCOPE_ctor(struct C *);
void STACKSCOPE_ctor_1AE4A0(struct C *, int) __asm__("_$_10STACKSCOPE");
void LoadXfmFromBrx(struct S *p, void *q);
void CLoadCollectorArray_ctor(struct DA *pda);
void LoadXfmChildObjectsFromBrx(struct S *p, void *q, struct DA *pda);
int IsBasicDerivedFrom__FP5BASIC3CID(void *p, int n);
// EXPLG::LoadFromBrx: XFM load, then its child objects; the explosions (CID 0x1E and derived) become the group's
// children, anything else gets vtable slot 9 (inferred: removal). Every child is flagged (LO bit 57).
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
    p->children = (__typeof__(p->children))PvAllocMsg(&msg, da.c * 4, 0x10);
    for (i = 0; i < da.c; i++) {
        struct O *o = da.a[i];
        o->fl |= 0x200000000000000L;
        if (!IsBasicDerivedFrom__FP5BASIC3CID(o, 0x1E)) {
            struct VTE *e = &o->vt[9];
            e->pfn((char *)o + e->d);
        } else {
            p->children[p->numChildren++] = o;
        }
    }
    STACKSCOPE_ctor_1AE4A0(&ctx, 2);
}
} } // namespace M_LoadExplgFromBrx

// Returns an explosion group's children: the array through the out parameter, the count as the result.
int EXPLG::GetChildren(LO ***childrenOut)
{
    EXPLGV *group = (EXPLGV *)this;
    *childrenOut = group->children;
    return group->numChildren;
}

namespace M_CloneExplg { extern "C" {
typedef struct MSG { int cb; int nKind; } MSG;
void *AllocCopyFromWorldHeap(MSG *pmsg, void *pv);
static inline void *PvReallocMsg(MSG *pmsg, void *pv, int cb, int nKind) { pmsg->cb = cb; pmsg->nKind = nKind; return AllocCopyFromWorldHeap(pmsg, pv); }
struct S { int pad[4]; void *psoParent; char pad2[0xE0 - 0x14]; int numChildren; void **children; };   /* EXPLG */
void CloneXfm(struct S *p, void *pv);
void *CloneLoWithParent__FP2LOP2SO(void *pv, void *p10);
// EXPLG clone: XFM clone, then a private copy of the child array with each child cloned under the same parent.
void CloneExplg(struct S *p, void *pv)
{
    MSG msg;
    int i;
    CloneXfm(p, pv);
    p->children = (__typeof__(p->children))PvReallocMsg(&msg, p->children, p->numChildren * 4, 0x10);
    for (i = 0; i < p->numChildren; i++)
        p->children[i] = CloneLoWithParent__FP2LOP2SO(p->children[i], p->psoParent);
}
} } // namespace M_CloneExplg

namespace M_ExplodeExplgExplso {
// ExplodeExplgExplso (Claude agent, lane D40): matched with Lilly's MATRIX3 operator* (src/game/engine/anim/tail/UpdateChainLink.cpp: rows of the LEFT
// matrix into qword locals, VU_VECTOR4 row temps) and her QwRotateVec(MATRIX3 *, qword), with px->mat as the left operand.
typedef int qword __attribute__((mode(TI)));
struct MATRIX4 { qword r[4]; };
struct MATRIX3 { qword r[3]; };
struct VEC { int x, y, z, w; } __attribute__((aligned(16)));
struct XF { qword pos; MATRIX3 localMatrix; VEC v; VEC w; };
/* EXPLSO: what to explode with (EXPLSO_* flags say which fields are set) */
struct P { int fl; void *reference; float scale, radius; qword vector, origin; int touch; XF *pxf; int count, pad3C; };
struct VTE { short d; short pad; void (*pfn)(void *, P *); };
struct O { int pad[3]; VTE *vt; char pad1[0x60 - 0x10]; MATRIX4 mat; };
struct S { char pad[0x60]; MATRIX4 mat; char pad1[0xE0 - 0xA0]; int c; O **apo; };
extern "C" { MATRIX4 InvertMatrix4Rigid(MATRIX4 *pm); }
MATRIX4 operator*(const MATRIX4 &a, const MATRIX4 &b) __asm__("__ml__FRC7MATRIX4T0");
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
// EXPLG explode: explodes every child explosion (vtable slot 51) with a copy of the EXPLSO whose transform is the
// child's placement relative to the group, composed with the caller's transform when it gave one; the group is the
// reference object unless the caller set one.
void ExplodeExplgExplso(S *group, P *explso)
{
    P childExplso = *explso;
    XF xf;
    childExplso.pxf = &xf;
    childExplso.fl |= EXPLSO_HAS_XF;
    if (childExplso.fl & EXPLSO_HAS_REFERENCE) {
    } else {
        childExplso.reference = group;
        childExplso.fl |= EXPLSO_HAS_REFERENCE;
    }
    for (int i = 0; i < group->c; i++) {
        O *child = group->apo[i];
        MATRIX4 mRel;
        mRel = InvertMatrix4Rigid(&group->mat) * child->mat;
        MATRIX3 &m3 = *(MATRIX3 *)&mRel;
        if (explso->fl & EXPLSO_HAS_XF) {
            XF *px = explso->pxf;
            xf.localMatrix = px->localMatrix * m3;
            xf.pos = VuAdd(px->pos, QwRotateVec(&px->localMatrix, mRel.r[3]));
            xf.v = px->v;
            xf.w = px->w;
        } else {
            xf.localMatrix = m3;
            xf.pos = mRel.r[3];
            xf.v.x = xf.v.y = xf.v.z = 0;
            xf.w.x = xf.w.y = xf.w.z = 0;
        }
        child->vt[51].pfn((char *)child + child->vt[51].d, &childExplso);
    }
}
} // namespace M_ExplodeExplgExplso

namespace M_HandleExplsMessage { extern "C" {
struct O { char pad[0x100]; void *pripg; void *pblipg; };   /* EXPLS: its particle groups (+0x100, +0x104) */
} } // namespace M_HandleExplsMessage
// EXPLS::HandleMessage: on message 1 (an object is going away) forgets its rip or blip group if it is that object.
void EXPLS::HandleMessage(MSGID msgid_m, void *pv)
{
    using namespace M_HandleExplsMessage;
    M_HandleExplsMessage::O *o = (M_HandleExplsMessage::O *)this;
    int msgid = (int)msgid_m;
    if (msgid == 1) {
        if (pv == o->pripg) {
            UnsubscribeLoObject(pv, o);
            o->pripg = 0;
        } else if (pv == o->pblipg) {
            UnsubscribeLoObject(pv, o);
            o->pblipg = 0;
        }
    }
}

namespace M_ExplodeExplsExplso { extern "C" {
struct VTE { short d; short pad; void (*pfn)(...); };
struct O { int a[3]; struct VTE *vt; };
// EXPLS explode: forwards to its own vtable slot 52 (fire) (inferred)
void ExplodeExplsExplso(struct O *o, int x)
{
    struct VTE *e = &o->vt[52];
    e->pfn((char *)o + e->d, x);
}
} } // namespace M_ExplodeExplsExplso

// Returns the EXPLS's sound effect, creating it on first use. Used by the class option table (five EXPLS options
// point at it), so level data can set the explosion's sound properties.
extern "C" SFX *EnsureExplsSfx(SFXHOLDER *explosionSound)
{
    if (explosionSound->sfx == 0) {
        explosionSound->sfx = NewSfx();
    }
    return explosionSound->sfx;
}

namespace M_FireExplsExplso { extern "C" {
// (Claude agent) MATCH: g8 if/else reload form, max via local h=0.5f, pfrm transform split into mul asm + vadd asm with t loaded late.
typedef int qword __attribute__((mode(TI)));
typedef struct EMITP { int reference; int shape; int render; int nextRender; int touch; } EMITP;   /* EMITB+0x08: the resolved objects */
typedef struct XF4 { qword v; } XF4;
typedef struct X4 { char pad[0xF4]; XF4 *p; } X4;   /* the curve shape object: its centre at ->p->v (inferred) */
typedef struct FRM { qword pos; qword m[3]; } FRM;
typedef struct EMITBV {
    int cref; int particleKind; EMITP *pemitp; int padC;
    int originKind;             /* +0x10 EMIT_ORIGIN */
    int normalKind;             /* +0x14 */
    int velocityKind;           /* +0x18 */
    FRM *pfrm;                  /* +0x1C the origin's local transform */
    int pad20[4];
    /* +0x30 the origin data: point, box corners, or an EMITMESH (its centre at +0x20) */
    union { qword v; qword *pm; EMITP *pemitp; struct { char pad[0x20]; qword v; } *pq; } origin;
    qword vector;               /* +0x40 */
    float speedMin, speedMax;   /* +0x50 */
    char pad58[0xD8 - 0x58];
    float ripScaleMin, ripScaleMax;   /* +0xD8 */
    qword blipScale[2];         /* +0xE0 (scaled with the explosion, at least half size) */
} EMITBV;
typedef struct SND { int id; float rangeMin, rangeMax, volume, pitch; } SND;   /* the EXPLS's SFX */
/* EXPLS */
typedef struct EMITTERV {
    char pad[0xE4]; EMITBV *pemitb; int padE8, padEC; SND *psnd;
    float countMin, countMax;   /* +0xF4 particles per explosion */
    int keepsGroups;            /* +0xFC emit into the EXPLS's own rip / blip groups (and subscribe it) */
    int pripg; int pblipg;      /* +0x100 */
} EMITTERV;
typedef struct EMITO { int grf; int reference; float scale; float radius; qword vector; qword origin; int touch; int pxf; int count; } EMITO;   /* EXPLSO */
typedef struct EMITR { int *ppblipg; int *ppripg; EMITTERV *subscriber; } EMITR;   /* EMITG */

extern char D_002BDEC0[];
extern float RandomFloatInRange__Fff(float lo, float hi);
extern float ClampToLimits__FP2LMf(void *p, float g);
typedef union V4 { qword q; float a[4]; } V4;
extern V4 func_00256CE8(float g) __asm__("__8VU_FLOATf");
extern V4 __9VU_VECTORRC6VECTOR(qword *pv);
extern qword __ml__FG8VU_FLOATG9VU_VECTOR(qword a, qword b);
extern void VECTOR_opAssign(qword *pv, qword a);
extern void EmitParticles__FiP5EMITBP5EMITG(int c, EMITBV *pb, EMITR *pr);
extern qword ConvertPosBetweenLos(int x, int y, qword v);
extern void *CreateSfxSound(int id);
extern void SetSndSourceLo(void *p, void *pso);
extern void SetSndSourcePos(void *p, qword v);
extern void SetSndRange(void *p, float a, float b);
extern void SetSndVolume(void *p, float g);
extern void SetSndPitch(void *p, float g);

// EXPLS fire: emits one explosion's particles from a copy of the EXPLS's EMITB with the EXPLSO's overrides
// (reference, direction, origin point or transform, touch object, count); a scaled explosion scales the count
// (capped at half the maximum), speed and blip size. Then plays the EXPLS's sound at the origin's centre (by
// origin kind, through the local transform, then into world space from the reference object).
void FireExplsExplso(EMITTERV *expls, EMITO *explso)
{
    EMITP emitp;
    EMITBV emitb;
    EMITR emitg;
    EMITR *pemitg;
    int count;
    V4 pos;
    V4 h;

    emitp = *expls->pemitb->pemitp;
    emitb = *expls->pemitb;
    emitb.pemitp = &emitp;
    if (explso->grf & EXPLSO_HAS_COUNT)
        count = explso->count;
    else
        count = (int)RandomFloatInRange__Fff(expls->countMin, expls->countMax);
    if (explso->grf & EXPLSO_HAS_REFERENCE)
        emitp.reference = explso->reference;
    if (explso->grf & EXPLSO_HAS_VECTOR)
        emitb.vector = explso->vector;
    if (explso->grf & EXPLSO_HAS_ORIGIN) {
        emitb.origin.v = explso->origin;
        emitb.originKind = EMIT_ORIGIN_POINT;
    } else if (explso->grf & EXPLSO_HAS_XF) {
        emitb.pfrm = (FRM *)explso->pxf;
    }
    if ((explso->grf & EXPLSO_HAS_SCALE) && emitb.particleKind == 0)   /* blips */ {
        float scale;
        float speedScale;
        int maxCount;
        if (explso->scale == 0.0f)
            scale = explso->radius * 0.01f;
        else
            scale = explso->scale;
        if (scale < 1.0f)
            count = (int)(count * scale);
        else
            count = (int)(count / scale);
        maxCount = (int)expls->countMax / 2;
        if (!(maxCount < count))
            count = maxCount;
        speedScale = ClampToLimits__FP2LMf(D_002BDEC0, scale);
        emitb.speedMin *= speedScale;
        emitb.speedMax *= speedScale;
        { float h = 0.5f; scale = (h < scale) ? scale : h; }
        VECTOR_opAssign(emitb.blipScale, __ml__FG8VU_FLOATG9VU_VECTOR(func_00256CE8(scale).q, __9VU_VECTORRC6VECTOR(emitb.blipScale).q));
    }
    if (emitb.particleKind == 1) {   /* rips */
        if (explso->grf & EXPLSO_HAS_RIP_SCALE)
            emitb.ripScaleMax = emitb.ripScaleMin = explso->scale;
        if (explso->grf & EXPLSO_HAS_TOUCH)
            emitb.pemitp->touch = explso->touch;
    }
    if (expls->keepsGroups) {
        emitg.ppblipg = &expls->pblipg;
        emitg.ppripg = &expls->pripg;
        emitg.subscriber = expls;
        pemitg = &emitg;
    } else {
        pemitg = 0;
    }
    if (emitp.reference == 0 && emitb.originKind != EMIT_ORIGIN_CURVE && !(explso->grf & (EXPLSO_HAS_SCALE | EXPLSO_HAS_XF)))
        emitp.reference = (int)expls;
    EmitParticles__FiP5EMITBP5EMITG(count, &emitb, pemitg);
    if (expls->psnd) {
        void *sound;
        switch (emitb.originKind) {
        case EMIT_ORIGIN_POINT:
            pos.q = emitb.origin.v;
            break;
        case EMIT_ORIGIN_BOX: {     /* the box centre */
            float half = 0.5f;
            {
                register qword hv __asm__("vf1");
                __asm__("qmtc2.ni %1, %0" : "=j"(hv) : "r"(half));
                h.q = hv;
                {
                    qword *pm = emitb.origin.pm;
                    register qword m1 __asm__("vf2") = pm[1];
                    register qword m0 __asm__("vf3") = pm[0];
                    register qword centre __asm__("vf1");
                    __asm__("vmulax.xyzw $ACC, %1, %3x\n\tvmaddx.xyzw %0, %2, %3x" : "=j"(centre) : "j"(m0), "j"(m1), "j"(hv));
                    pos.q = centre;
                }
            }
            break;
        }
        case EMIT_ORIGIN_CURVE:
            pos.q = ((X4 *)emitb.pemitp->shape)->p->v;
            break;
        case EMIT_ORIGIN_MESH:
            pos.q = emitb.origin.pq->v;
            break;
        }
        if (emitb.pfrm) {
            FRM *pf = emitb.pfrm;
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
        pos.q = ConvertPosBetweenLos(emitp.reference, 0, pos.q);
        sound = CreateSfxSound(expls->psnd->id);
        SetSndSourceLo(sound, 0);
        SetSndSourcePos(sound, pos.q);
        SetSndRange(sound, expls->psnd->rangeMin, expls->psnd->rangeMax);
        SetSndVolume(sound, expls->psnd->volume);
        SetSndPitch(sound, expls->psnd->pitch);
    }
}
} } // namespace M_FireExplsExplso



/* The linkonce copies 0x2584A8..0x2599C8: the EMITTER and EXPLO instances of sly2/emitter.h's member templates
   (each pair is one template; the accessors between them are instantiated by g_aeopid, class_registry). An explicit
   instantiation emits each in its own .gnu.linkonce.t.<symbol>, as the original's implicit ones were. */
/* sly2/emitter.h's empty reference blocks: EMITTER's (0x2CE4A0), then EXPLO's (0x2CE4B8), weak in .gnu.linkonce.d. */
template <class T> void *EMITBREFS<T>::s_apvNull[5] = {0, 0, 0, 0, 0};
template void *EMITBREFS<EMITTER>::s_apvNull[5];
template void *EMITBREFS<EXPLO>::s_apvNull[5];
template void LoadEmitblipColorsFromBrx(EMITTER *, CBinaryInputStream *);   /* 0x2584A8 */
template void LoadEmitterFromBrx(EMITTER *, CBinaryInputStream *);          /* 0x258598, EMITTER slot 14 */
template void CloneEmitter(EMITTER *, LO *);                                /* 0x258620, slot 41 */
template EMITB *PemitbEnsure(EMITTER *, ENSK);                              /* 0x258658 */
template void func_002586D8(EMITTER *);                                     /* 0x2586D8 */
template CLASSINFO *GetEmitterClassDesc_linkonce(EMITTER *);                /* 0x258F30 */
template void LoadEmitblipColorsFromBrx(EXPLO *, CBinaryInputStream *);     /* 0x258F40 */
template void LoadEmitterFromBrx(EXPLO *, CBinaryInputStream *);            /* 0x259030, EXPLO slot 14 */
template void CloneEmitter(EXPLO *, LO *);                                  /* 0x2590B8, slot 41 */
template EMITB *PemitbEnsure(EXPLO *, ENSK);                                /* 0x2590F0 */
template void func_002586D8(EXPLO *);                                       /* 0x259170, slot 15 */
template CLASSINFO *GetEmitterClassDesc_linkonce(EXPLO *);                  /* 0x2599C8 */

// ---- Data owned by this TU, in address order (docs/guides/tu-merging.md "Data"). Defined after the functions
// so no function sees a definition (keeps the code as matched); the namespaced declarations above refer to these.
/* The shared defaults emitter.h's accessors reset a slot from (0x2BDDF0..0x2BDEB0, the head of emitter's .data; the
   payload shapes as emitter.h declares them). The two 0x40-byte records and the EMITV2 (whose first word is all
   ones, not a float literal) are given as words under their asm labels: record 0 and record 1 lay their payloads out
   differently from emitter.h's EMITV view (floats in the OID and pad fields), so the view's field names do not fit. */
EMITV4 D_002BDDF0 = {{0.0f, 0.0f, 0.0f, 0.0f}};
float D_002BDE00 = 1.0f;
namespace EMIT_DEFAULTS {
extern "C" unsigned int r_D_002BDE10[16] __asm__("D_002BDE10") __attribute__((aligned(16))) = {   /* record 0 */
    0, 0, 0, 0, 0xFFFFFFFF, 0xFFFFFFFF, 0, 0, 0x41C80000 /* 25.0f */, 0, 0, 0, 0, 0, 0, 0};
extern "C" unsigned int r_D_002BDE50[2] __asm__("D_002BDE50") = {0xFFFFFFFF, 0x41200000 /* 10.0f */};
}
int D_002BDE58 = 0;
EMITV3 D_002BDE60 = {{25.0f, 0.8f, 0.8f}};
namespace EMIT_DEFAULTS {
extern "C" unsigned int r_D_002BDE70[16] __asm__("D_002BDE70") __attribute__((aligned(16))) = {   /* record 1 */
    0xFFFFFFFF, 0, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x41C80000 /* 25.0f */, 0x3F800000 /* 1.0f */,
    0x3F800000 /* 1.0f */, 0x3F800000 /* 1.0f */, 0, 0, 0, 0, 0, 0, 0};
}
int D_002BDEB0[4] = {75, 77, 78, 76};
LM D_002BDEC0 = {0.5f, 2.0f};

// ---- Pointer constants and records, by address name (docs/guides/tu-merging.md "Data"): words as the ELF has
// them, pointers through asm-labelled declarations. Non-virtual pointer-to-member constants are 8 bytes
// {delta 0, index -1, pfn}; the definitions sit in their own namespace so no declaration above has to agree.
namespace ANPTR_emitter {
void p_func_00159968() __asm__("AttachEmitterToReferencedObject");
extern "C" {
extern const unsigned int D_002A5140[2] = {0xFFFF0000, (unsigned int)p_func_00159968};
}
} // namespace ANPTR_emitter
// The three FLT_MAX words follow the record in .rodata (definition order is emission order).
extern const float D_002A5148 = 3.40282347e+38f;
extern const float D_002A514C = 3.40282347e+38f;
extern const float D_002A5150 = 3.40282347e+38f;
