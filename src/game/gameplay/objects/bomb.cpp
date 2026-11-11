// bomb.cpp -- BOMB (an SO): init/load/clone, priming and detonation (the blast: objects hit within the radii get
// a 0x1C-style impact message with a push away from the blast; the bomb then moves to the detonation pose and
// removes itself), custom contacts it adds while flying near marked objects, throwing, absorbing a hit, and the
// detonation-pose setup. Entry points: the BOMB vtable (InitBomb, PostBombLoad, HandleMessage, UpdateBomb, ResetBomb,
// HandleBombContact, AddBombCustomXps, ShouldBombIgnoreIntersection, TakeBombDamage, CloneBomb, ApplyBombThrow), PrimeBomb
// and DetonateBomb (also used by MISSILE). VU0 macro-mode asm with register pins.
// TU 0x13A9E8..0x13BDF0 (22 functions, address order).
// SLY2_ALIAS: __static_initialization_and_destruction_0=StaticInit_bomb
// SLY2_ALIAS: _GLOBAL_$I$*=GlobalConstructors_bomb
#include "sly2/slotheap.h"
#include "sly2/vu0.h"
#include "sly2/clock.h"

// 0x13BC98 StaticInit_bomb and GlobalConstructors_bomb: g++-generated for the six engine slot pools.
SLY2_INSTANTIATE_SLOT_POOLS()

#include "sly2/sw.h"
#include "sly2/classes.h"
typedef union { qword q; float f[4]; } U;
/* g++ 2.95 vtable entry (the blast listener's slot 51 is called by hand: its class is not known). */

/* The bomb's XA (+0xE4: its position at +0x50) and XP (+0xEC: radius at +0x30, owner at +0x60, contact list at +0x64). */
struct XPQ;
typedef struct XPL { XPQ *pq; struct XPL *next; } XPL;
struct XEC { char pad[0x30]; float g30; char pad34[0x2C]; void *pso60; XPL *pl; };
/* A custom contact (0x30): position, normal, the normal's dot as float bits, the object. */
struct CXP { qword pos; qword normal; int n20; SO *plo; int pad[2]; };
typedef char assert_CXP_size[sizeof(CXP) == 0x30 ? 1 : -1];
struct POV;
struct LOROOTV { void *plo; };
struct BOMBV : SO   /* classes.h BOMB : SO; its own fields */
{
    int k;                     /* +0x150 0 armed, 1 absorbing... (0x13AEB0, TakeBombDamage) */
    float tDetonate;           /* +0x154 */
    float detonateSpeed;       /* +0x158 detonates on impact above this speed (UpdateBomb) */
    float sRadius;             /* +0x15C blast radius */
    float sRadiusHero;         /* +0x160 blast radius against the hero (CID 0x34) */
    int noBlast;               /* +0x164 detonation without blast damage */
    POV *blastListener;        /* +0x168 told about the blast (vtable slot 51) */
    float blastListenerArg;    /* +0x16C passed with it (inferred) */
    int detonationPoseMode;    /* detonation pose: 0/2 rotate to the hit normal, 1 face the velocity, 3 keep */
    int isDetonationPrepared;  /* +0x174 */
    int pad178[2];
    qword posDetonate;         /* +0x180 */
    MATRIX3Q matDetonate;            /* +0x190 */
    qword velocityAtDetonation;        /* +0x1C0 */
    qword angularVelocityAtDetonation; /* +0x1D0 */
    int makesNoise;            /* +0x1E0 detonation creates a noise stimulus (NewNoise), inferred */
    int detonatesOnContact;    /* +0x1E4 (inferred: kind-1 bombs detonate on contact only when set) */
    LO *ploStuckTo;            /* +0x1E8 the object it is stuck to (classes.h BOMB::ploStuckTo; ApplyBombThrow) */
    int zapKind;               /* +0x1EC zap kind of the blast message (0x100) */
    int blastDamage;           /* +0x1F0 damage of the blast message (-10) */
    int fCustomXps;            /* +0x1F4 */
    int cxp;
    CXP *axp;                  /* +0x1FC */
    BOMBV *thrower;            /* +0x200 classes.h BOMB::thrower: itself until thrown; contacts with its root are ignored */
};
typedef char assert_BOMB_thrower[(unsigned int)&((BOMBV *)0)->thrower == 0x200 ? 1 : -1];
/* Objects of the SO list D_003E13B4 (UpdateBombCustomXps) and their marker LOs. */
struct NV { char pad[0x60]; SO *plo; };
struct SOV : BASIC { SOV *pparent; LOROOTV *ploroot; char pad1[0x1C - 0x18]; void *unknown1C; void *unknown20; void *unknown24; void *unknown28; char pad2C[0x34 - 0x2C]; void *firstRunCallback; char pad38[0x3C - 0x38]; void *proxyRecord; char pad40[0x54 - 0x40]; void *transforms; void *positionXf; void *rotationXf; void *rootTransform; void *unknown64; void *unknown68; char pad6C[0x70 - 0x6C]; void *ploRemovedFirst; char pad74[0x80 - 0x74]; void *unknown80; char pad84[0xA8 - 0x84]; void *unknownA8; char padAC[0xB0 - 0xAC]; void *unknownB0; char padB4[0xBC - 0xB4]; void *unknownBC; void *unknownC0; void *unknownC4; void *unknownC8; char padCC[0xD8 - 0xCC]; void *unknownD8; void *unknownDC; SOV *pnext; int xE4; int grfE8; union { NV *pn; XEC *pxp; }; int xF0; };
/* The SO list head D_003E13B4. */
struct SOLIST { SOV *p; };
struct HITB { qword pos; qword a; qword normal; qword b; qword c; qword d; };
struct HIT48 { int a[12]; };
/* AddBombCustomXps's contact request and its object. */
struct CREQ { int x0; SOV *po; };
/* A bounding-sphere hit (CollectBombVictims). */
struct XAV { int x0; SOV *po; int x8; int xC; char pad[0x74 - 0x10]; int planeCount; };   /* a SOPART */
struct AREQV { int cb; int cbAlign; };
/* 8 bytes read with ldl/ldr (alignment 1). */
/* DetonateBomb's impact message and the message to +0x168. */
struct POV { void *scriptFrame; void *scriptVariables; void *firstSubscriber; VTE *pvt; int x10; POV **pp14; };
struct MSG2 { int k; BOMBV *p; float g8; float gC; };
struct FRAME { int a[2]; }; // FrameInit = ctor, FrameFree = dtor(this, 2)

/* bomb's .data (address order): the default radius, the four classes that defuse a chain, the gravity scalar. */
float D_002BC9C8 = 200.0f;
extern const float D_002A4B18;   /* defined below where the ELF has it (.rodata order) */
extern const float D_002A4B1C;   /* defined below where the ELF has it (.rodata order) */
extern const float D_002A4B20;   /* defined below where the ELF has it (.rodata order) */
extern qword D_002A2DA0;
extern const LONGU D_002A4B10;   /* an 8-byte callback record (a pointer to member function) read as one packed long: see B4 in the merge guide */
int D_002BC9D0[4] = {0x33, 0x64, 0x4F, 0x67};
extern qword D_002A2DD0;
extern "C" {
extern qword g_normalZ;
int D_002BC9E0 = 0x43FA0000;   /* 500.0f, read as its bit pattern */
void InitSo(BOMBV *);
void CloneSo(BOMBV *, BOMBV *);
void PostSoLoad(BOMBV *);
void UpdateSo(BOMBV *p);
void *AllocFromWorldHeap(AREQV *);
void HandleBasicMessage(BOMBV *, int, void *);
void UpdateSwPox(SW *pg, SOTREE *pa, SOTREE *pb, int n);
qword GetLoWorldPos(void *p);
int ClipEdgeToSphere(qword a, qword b, qword c, int n, HITB *phit, float g);
void AddSoCustomXps(BOMBV *p, CREQ *ps, int a2, int a3, int a4, int a5);
void AddXp(SW *pg, SOPART *ppart, CREQ *ps, qword a, qword b, int k, int x, int y, float g, int z);
void AdjustSoXps(BOMBV *p);
int IsBasicDerivedFrom__FP5BASIC3CID(void *p, int n);
int IsPoPlayable(BOMBV *p);
void PostSwCallback(SW *pg, BOMBV *p, long k, void *x);
void SetLoParent(BOMBV *, LO *, int);
void SetSoPosXfVelocityLocal(BOMBV *, qword);
void SetSoRotXfVelocityLocal(BOMBV *, qword);
void SetSoCnstrForce(BOMBV *, int);
void SetSoCnstrTorque(BOMBV *, int);
int CanSoTakeDamage(BOMBV *p, ZPR *pm);
MATRIX3Q *GetLoWorldMatrix(BOMBV *);
void BuildOrthonormalMatrixZ(MATRIX3Q *, qword, qword);
void BuildRotateVectorsMatrix__FP6VECTORT0P7MATRIX3(MATRIX3Q *, qword, qword);
void ShouldKeepSoNotSelf();
void IntersectSwBoundingSphere(SW *pg, qword q, void (*pfn)(), SOTREE *px, float g, int *pc, XAV ***paps);
void *AllocFromStack(AREQV *preq);
int IntersectSphereBsp(qword q, XAV *ps, int n, int a, int b, int c, float g, HIT48 *phit);
void qsort(void *, int, int, int (*)(const void *, const void *));
void *func_002333B4(void *pv, int b, int cb) __asm__("memset");
int DeliverZapToSo(ZPR *pmsg);
int *NewNoise(STIMK *pstimk, qword pos, void *pv);
void *FrameInit(FRAME *) __asm__("STACKSCOPE_ctor");
void FrameFree(FRAME *, int) __asm__("_$_10STACKSCOPE");
void SO_ResetForProxy(BOMBV *);
void SetSoTreeFlag2(BOMBV *, int);
int ShouldSoIgnoreIntersection(BOMBV *pbomb, SOV *pso);
int MaxInt(int r, int n);
void RefreshSwObjectCollisionFilter(SW *pg, BOMBV *p);
void DetonateBomb(BOMBV *p);
void UpdateBombCustomXps(BOMBV *p);
int IsBombChainFreeOfExcludedClasses(BOMBV *p, SOV *pso);
void PrepareBombDetonation(BOMBV *p, qword pos, qword dir);
void CollectBombVictims(BOMBV *p, int *pc, SOV ***papo);
int CompareBombVictims(const void *, const void *);
void SortUniqueBombVictims(BOMBV *p, int *pc, SOV **apo);
void SetBombOwner(BOMBV *p, BOMBV *pbombOwner);
}
void PrimeBomb(BOMBV *pbomb, float dt) __asm__("PrimeBomb__FP4BOMBf");

// BOMB vtable slot 7: InitSo, tDetonate=FLT_MAX, radii, zap defaults, flag, thrower=self.
// Also known as: InitBomb (upstream readable / Sly 1 name; InitBomb__FP4BOMB).
extern "C" void InitBomb(BOMBV *bomb) {
    sptr x;
    InitSo(bomb);
    bomb->tDetonate = 3.40282346638528859812e+38f;
    bomb->sRadius = D_002BC9C8;
    bomb->sRadiusHero = D_002BC9C8;
    bomb->makesNoise = 1;
    bomb->zapKind = 0x100;
    bomb->blastDamage = -10;
    x = (sptr)bomb->ploupd;
    bomb->k = 0;
    if ((x ^ 1) & 1) ((LOUPD *)x)->grf |= 2;
    else bomb->unknown40 = x | 2;
    bomb->thrower = bomb;
}

// BOMB vtable slot 76: SO Clone then reset the thrower to self (SetBombOwner). (inferred)
extern "C" void CloneBomb(BOMBV *bomb, BOMBV *source)
{
    CloneSo(bomb, source);
    SetBombOwner(bomb, bomb);
}

// BOMB vtable slot 15: PostSoLoad, and with fCustomXps (+0x1F4) allocate the custom-XP buffer (0xC0).
// Also known as: PostBombLoad (upstream readable / Sly 1 name; PostBombLoad__FP4BOMB).
extern "C" void PostBombLoad(BOMBV *bomb) {
    PostSoLoad(bomb);
    if (bomb->fCustomXps) { AREQV k; k.cb = 0xc0; k.cbAlign = 0x10; bomb->axp = (CXP *)AllocFromWorldHeap(&k); }
}

// BOMB vtable slot 5: base handler; on msg 4 about itself while not primed, PrimeBomb(0).
// Also known as: HandleBombMessage (upstream readable / Sly 1 name; HandleBombMessage__FP4BOMB5MSGIDPv).
void BOMB::HandleMessage(MSGID msg_m, void *pm_m) { BOMBV *bomb = (BOMBV *)this; int msg = (int)msg_m; CREQ *pm = (CREQ *)pm_m;
    HandleBasicMessage(bomb, msg, pm);
    if (msg == 4 && (BOMBV *)pm->po == bomb && bomb->k == 0) PrimeBomb(bomb, 0.0f);
}

// Length of a vector as a float (VU0 sqrt).
static __inline__ float GLength(qword v)
{
    qword d;
    float g;
    d = VuDot(v, v);
    __asm__("vsqrt Q, %0x\n"
        "vwaitq\n"
        "vaddq.x %0, $vf0, Q" : "+j"(d));
    g = VuToFloat(d);
    return g;
}
// BOMB vt16: UpdateSo; if timer expired or speed above threshold DetonateBomb, else UpdateBombCustomXps when enabled.
// Also known as: UpdateBomb (upstream readable / Sly 1 name; UpdateBomb__FP4BOMBf).
extern "C" void UpdateBomb(BOMBV *bomb)
{
    UpdateSo(bomb);
    if (bomb->tDetonate <= g_clock.time)
        goto out;
    {
        qword v;
        int f;
        v = bomb->GetVelocity();   /* assignments, not initializers: C++ forbids the goto past initializations */
        f = GLength(v) < bomb->detonateSpeed;
        if (f) {
        out:
            DetonateBomb(bomb);
        } else if (bomb->fCustomXps) {
            UpdateBombCustomXps(bomb);
        }
    }
}

// Drop previous forced pairs (UpdateSwPox k=2), then pair with candidate SOs carrying flag 0x100. (inferred)
// Also known as: SweepProjectile (upstream readable / Sly 1 name; SweepProjectile__FP3ALO).
extern "C" void UpdateBombCustomXps(BOMBV *bomb)
{
    int i;
    SOV *pso;
    int unused[4];
    HITB hit;
    for (i = 0; i < bomb->cxp; i++)
        UpdateSwPox(&g_sw, bomb->physicsTree, bomb->axp[i].plo->physicsTree, 2);
    bomb->cxp = 0;
    for (pso = (*(SOLIST *)&g_sw.dlTarget).p; pso; pso = pso->pnext) {
        SO *plo;
        SOV *pp;
        if (!pso->xF0)
            continue;
        if (!(pso->grfE8 & 0x100))
            continue;
        plo = 0;
        for (pp = pso->pparent; pp; pp = pp->pparent) {
            if (pp->pn) {
                plo = pp->pn->plo;
                break;
            }
        }
        if (!plo)
            continue;
        if (!(uchar)plo->isInWorld)
            continue;
        {
            qword a = GetLoWorldPos(pso);
            SOTREE *px = plo->physicsTree;
            qword b = *(qword *)bomb->parts->matWorld.mat[3];
            if (ClipEdgeToSphere(a, b, GetLoWorldPos(bomb), 1, &hit, px->boundsRadius)) {
                CXP *pe;
                qword d;
                register qword vp __asm__("vf2");
                register qword vn __asm__("vf1");
                register int n __asm__("$2");
                UpdateSwPox(&g_sw, bomb->physicsTree, plo->physicsTree, 1);
                pe = &bomb->axp[bomb->cxp++];
                pe->plo = plo;
                __asm__("" : "+r"(pe));
                vp = hit.pos;
                pe->pos = vp;
                vn = hit.normal;
                d = VuDot(vp, vn);
                __asm__("qmfc2.ni %0, %1" : "=r"(n) : "j"(d));
                pe->normal = vn;
                pe->n20 = n;
                if (bomb->cxp >= 4)
                    break;
            }
        }
    }
}

// BOMB vt60: base SO hook, then for a custom-xp entry matching the other object add an XP (AddXp type 9). (inferred)
// Also known as: MarkProjectileSweep (upstream readable / Sly 1 name; MarkProjectileSweep__FP3ALOP3SOEiiii).
extern "C" void AddBombCustomXps(BOMBV *bomb, CREQ *ps, int a2, int a3, int a4, int a5)
{
    AddSoCustomXps(bomb, ps, a2, a3, a4, a5);
    if (bomb->fCustomXps) {
        CXP *pe;
        for (pe = bomb->axp; pe < bomb->axp + bomb->cxp; pe++) {
            if ((void *)pe->plo == ps->po->pxp->pso60) {
                AddXp(&g_sw, bomb->parts, ps, pe->pos, pe->normal, 9, 0, 0, *(float *)&pe->n20, a5);
                return;
            }
        }
    }
}

typedef struct XPE { BOMBV *object; void *x; int pad[14]; } XPE;
typedef struct XPQ { int x0, x4; XPE ae[2]; char pad88[0x24]; struct XPQ *pqNext; } XPQ;
#define AE(q) ((q)->ae)
// Index (0/1) of the other end of a contact pair, relative to this bomb.
static inline int IOther(XPQ *pq, BOMBV *bomb)
{
    int i = bomb != AE(pq)[0].object;
    i = 1 - i;
    return i;
}
// Length of a vector, in lane x.
static inline qword Length(qword a)
{
    qword r, one;
    __asm__("vmul.xyz %0, %2, %2\n\tvaddw.x %1, $vf0, $vf0w\n\tvadday.x $ACC, %0, %0y\n\tvmaddz.x %0, %1, %0z\n\t"
            "vsqrt $Q, %0x\n\tvwaitq\n\tvaddq.x %0, $vf0, $Q"
        : "=j"(r), "=j"(one) : "j"(a));
    return r;
}
// Length of a vector, in lane x (dot product form).
static __inline__ qword VLenA(qword c)
{
    qword l;
    l = VuDot(c, c);
    __asm__("vsqrt Q, %1x\n\tvwaitq\n\tvaddq.x %0, $vf0, Q" : "=j"(l) : "j"(l));
    return l;
}
// Vector divided by lane x of f.
// Lane x of a 128-bit register as a float.

// Unit vector of v, or def for a near-zero vector.
static inline qword NormalizeSafe(qword v, qword def)
{
    qword len = Length(v);
    if (QwordX(len) < 0.0001f)
        return def;
    return VuDivByX(v, len);
}
// .rodata order (docs/guides/tu-merging.md "Phase 5: data order"): a named constant is emitted where it is
// defined, a function's literals and jump tables with the function, so each constant is defined where the ELF
// has it among the functions, under an asm label (no function sees a definition under the name it reads).
// D_002A4B10: a virtual pointer-to-member constant, 8 bytes {delta 0, vtable index, vptr offset 0xC} (tu-merging.md "Data").
namespace RO_ORDER { extern const unsigned int r_D_002A4B10[2] __asm__("D_002A4B10") = {0x4E0000, 0xC}; }
namespace RO_ORDER { extern const float r_D_002A4B18 __asm__("D_002A4B18") = 3.40282347e+38f; }
// BOMB vtable slot 58: after base, choose best contact/target and prime or prepare detonation (state machine on +0x170 kinds
// 0/1/2). (inferred)
// Also known as: ProjectileContacts (upstream readable / Sly 1 name; ProjectileContacts__FP3ALO).
extern "C" void HandleBombContact(BOMBV *bomb)
{
    qword pos;
    float gBest;
    XPQ *pqBest;
    XPL *pl;
    XPQ *pq;

    AdjustSoXps(bomb);
    switch (bomb->k) {
    case 0:
    case 2:
        if (bomb->isDetonationPrepared)
            return;
        break;
    case 1:
        if (!bomb->detonatesOnContact)
            return;
        if (bomb->ploStuckTo)
            return;
        break;
    }
    pos = *(qword *)bomb->parts->matWorld.mat[3];
    gBest = D_002A4B18;
    pqBest = 0;
    for (pl = (XPL *)bomb->physicsTree->pxplistFirst; pl; pl = pl->next) {
        for (pq = pl->pq; pq; pq = pq->pqNext) {
            float g = QwordX(Length(VuSub(*(qword *)AE(pq)[0].pad, pos)));
            int f;
            if (gBest <= g)
                continue;
            f = 0;
            switch (bomb->k) {
            case 0:
                f = 1;
                break;
            case 2: {
                BOMBV *pOther = AE(pq)[IOther(pq, bomb)].object;
                if (IsBasicDerivedFrom__FP5BASIC3CID(pOther, 0x32)) {
                    f = 0;
                    if (IsPoPlayable(pOther))
                        f = 1;
                }
                break;
            }
            case 1:
                if (IsBombChainFreeOfExcludedClasses(bomb, (SOV *)AE(pq)[IOther(pq, bomb)].x))
                    f = 1;
                break;
            }
            if (f) {
                gBest = g;
                pqBest = pq;
            }
        }
    }
    if (!pqBest)
        return;
    switch (bomb->k) {
    case 0:
    case 2: {
        qword posT = *(qword *)AE(pqBest)[0].pad;
        qword v = bomb->GetVelocity();
        register qword n __asm__("vf1");
        register qword kDef __asm__("$6");
        qword l;
        float gl;
        __asm__("" : "=r"(kDef) : "0"(D_002A2DA0));
        l = VLenA(v);
        gl = VuToFloat(l);
        if (gl < 1e-4f) {
            n = kDef;
        } else {
            qword rn;
            __asm__("vdiv Q, $vf0w, %2x\n\tvwaitq\n\tvmulq.xyzw %0, %1, Q" : "=j"(rn) : "j"(v), "j"(l));
            n = rn;
        }
        PrepareBombDetonation(bomb, posT, n);
        PrimeBomb(bomb, 0.0f);
        break;
    }
    case 1:
        PostSwCallback(&g_sw, bomb, D_002A4B10.l, AE(pqBest)[IOther(pqBest, bomb)].x);
        break;
    }
}

// 0 if any object in the chain derives from one of 4 classes (D_002BC9D0), else 1. (inferred)
// Also known as: FStickyProjectileParent (upstream readable / Sly 1 name; FStickyProjectileParent__FP3ALOPv).
extern "C" int IsBombChainFreeOfExcludedClasses(BOMBV *bomb, SOV *pn) {
    unsigned int i;
    for (; pn; pn = pn->pparent) {
        for (i = 0; i < 4; i++) {
            if (IsBasicDerivedFrom__FP5BASIC3CID(pn, D_002BC9D0[i])) return 0;
        }
    }
    return 1;
}

// BOMB vt77: remember the object it sticks to (+0x1E8), reparent to it, zero velocities, lock force/torque constraints, fire Splice event
// 0x1B. (inferred)
// Also known as: AttachStickyProjectile (upstream readable / Sly 1 name; AttachStickyProjectile__FPvT0).
extern "C" void ApplyBombThrow(BOMBV *bomb, LO *q) {
    bomb->ploStuckTo = q;
    SetLoParent(bomb, q->oid != 1 ? q : 0, 3);
    SetSoPosXfVelocityLocal(bomb, D_002A2DD0);
    SetSoRotXfVelocityLocal(bomb, D_002A2DD0);
    SetSoCnstrForce(bomb, 1);
    SetSoCnstrTorque(bomb, 1);
    bomb->HandleSpliceEvent(0x1b, 0, 0, 0, 0);
}

// Length of a vector, in lane x.
static __inline__ qword VLen(qword c)
{
    qword l;
    l = VuDot(c, c);
    __asm__("vsqrt Q, %1x\n\tvwaitq\n\tvaddq.x %0, $vf0, Q" : "=j"(l) : "j"(l));
    return l;
}
// BOMB vt75 TakeDamage: refuse while state 1; else on knock-back zaps push the bomb along the hit normal. (inferred)
// Also known as: FAcceptContactProjectile (upstream readable / Sly 1 name; FAcceptContactProjectile__FP3ALOP5CTMSG).
extern "C" int TakeBombDamage(BOMBV *bomb, ZPR *pm)
{
    int r;
    if (bomb->k == 1)
        return 0;
    r = CanSoTakeDamage(bomb, pm);
    if (r && (pm->grfzpr & 8)) {
        U u;
        U *pu;
        register qword n __asm__("vf1");
        qword k;
        qword c = pm->velocity;
        qword l;
        float gl;
        __asm__("" : "=r"(k) : "0"(g_normalZ));
        l = VLen(c);
        gl = VuToFloat(l);
        pu = &u;
        if (gl < 1e-4f) {
            n = k;
        } else {
            qword rn;
            __asm__("vdiv Q, $vf0w, %2x\n\tvwaitq\n\tvmulq.xyzw %0, %1, Q" : "=j"(rn) : "j"(c), "j"(l));
            n = rn;
        }
        pu->q = n;
        PrepareBombDetonation(bomb, GetLoWorldPos(bomb), u.q);
        if (IsBasicDerivedFrom__FP5BASIC3CID(pm->ploSource, 0x4F))
            PrimeBomb(bomb, 0.25f);
        else
            PrimeBomb(bomb, 0.0f);
    }
    return r;
}

// 3x3 matrix times vector.
static __inline__ qword MulV(MATRIX3Q *pm, qword v)
{
    qword o;
    o = VuMulMatrix3(pm->r[0].q, pm->r[1].q, pm->r[2].q, v);
    return o;
}
// Stores pm times the bomb's world rotation as its detonation orientation (matDetonate).
static __inline__ void MulStore(BOMBV *bomb, MATRIX3Q *pm)
{
    MATRIX3Q t, u;
    MATRIX3Q *pb = GetLoWorldMatrix(bomb);
    u.r[0].q = t.r[0].q = MulV(pm, pb->r[0].q);
    u.r[1].q = t.r[1].q = MulV(pm, pb->r[1].q);
    t.r[2].q = u.r[2].q = MulV(pm, pb->r[2].q);
    bomb->matDetonate = t;
}
// Once: record detonation pos (+0x180), save v/w, orient by mode (+0x170) using the impact normal.
// Also known as: RecordHit (upstream readable / Sly 1 name; RecordHit__FP3ALOG9VU_VECTORT1).
extern "C" void PrepareBombDetonation(BOMBV *bomb, qword pos, qword q)
{
    MATRIX3Q m;
    if (bomb->isDetonationPrepared)
        return;
    bomb->posDetonate = pos;
    bomb->isDetonationPrepared = 1;
    bomb->velocityAtDetonation = bomb->GetVelocity();
    bomb->angularVelocityAtDetonation = bomb->GetAngularVelocity();
    switch (bomb->detonationPoseMode) {
    case 0:
        BuildRotateVectorsMatrix__FP6VECTORT0P7MATRIX3(&m, GetLoWorldMatrix(bomb)->r[2].q, q);
        MulStore(bomb, &m);
        break;
    case 1:
        BuildOrthonormalMatrixZ(&m, bomb->velocityAtDetonation, g_normalZ);
        bomb->matDetonate = m;
        break;
    case 2:
        BuildRotateVectorsMatrix__FP6VECTORT0P7MATRIX3(&m, GetLoWorldMatrix(bomb)->r[0].q, q);
        MulStore(bomb, &m);
        break;
    case 3:
        bomb->matDetonate = *GetLoWorldMatrix(bomb);
        break;
    }
}

// tDetonate = min(tDetonate, now + dt).
// Also known as: ExpireIn (upstream readable / Sly 1 name; ExpireIn__FP3ALOf).
void PrimeBomb(BOMBV *bomb, float f) { bomb->tDetonate = (bomb->tDetonate < g_clock.time + f) ? bomb->tDetonate : g_clock.time + f; }

// (Claude agent) IntersectSphereBsp takes its float before the HIT* (see AGENT_GUIDE "Float vs. int parameter order").
// Radius: fold turns `b >= a ? b : a` into MAX(b, a) only with both values in locals (a loaded first).
// Objects within max(radius, radiusJt) of the detonation point (IntersectSwBoundingSphere), filtered by line of
// sight, sorted/uniqued. (inferred)
// Also known as: AploVictims (upstream readable / Sly 1 name; AploVictims__FP3ALOP6VECTORPv).
extern "C" void CollectBombVictims(BOMBV *bomb, int *pc, SOV ***papo)
{
    AREQV req;
    int c;
    XAV **aps;
    int n;
    SOV **apo;
    int i;
    { float a = bomb->sRadius; float b = bomb->sRadiusHero; IntersectSwBoundingSphere(&g_sw, bomb->posDetonate, ShouldKeepSoNotSelf, bomb->physicsTree, b >= a ? b : a, &c, &aps); }
    if (c == 0) {
        *pc = 0;
        *papo = 0;
        return;
    }
    req.cb = c * 4;
    req.cbAlign = 0x10;
    apo = (SOV **)AllocFromStack(&req);
    n = 0;
    for (i = 0; i < c; i++) {
        XAV *ps = aps[i];
        HIT48 hit;
        void *pso60 = ps->po->pxp->pso60;
        if (ps->xC == 1)
            continue;
        if (pso60 == bomb->physicsTree->root)
            continue;
        if (ps->planeCount && !IntersectSphereBsp(bomb->posDetonate, ps, 0x7FFFFFFF, 0, 0, 1, IsBasicDerivedFrom__FP5BASIC3CID(pso60, 0x34) ? bomb->sRadiusHero : bomb->sRadius, &hit))
            continue;
        apo[n] = ps->po;
        n++;
    }
    SortUniqueBombVictims(bomb, &n, apo);
    *pc = n;
    *papo = apo;
}

// qsort comparator (pointer difference / element size). (inferred)
extern "C" int CompareBombVictims(const void *pv0, const void *pv1) {
    return (int)(((int)pv1 - (int)pv0) * 0x3CF3CF3D) >> 4;
}

// qsort then remove duplicates in place. (inferred)
extern "C" void SortUniqueBombVictims(BOMBV *unused, int *pc, SOV **a) {
    int n = *pc;
    SOV **pDst, **pSrc, **pMax;
    if (n < 2) return;
    qsort(a, n, 4, CompareBombVictims);
    pDst = a;
    pMax = a + n;
    for (pSrc = a; pSrc < pMax; pSrc++) {
        if (*pDst != *pSrc) *++pDst = *pSrc;
    }
    *pc = pDst - a + 1;
}

// (Claude agent) frame declared at function scope with explicit init/free calls (free = dtor with in-charge 2) so msg2 reuses msg's slot.
namespace RO_ORDER { extern const float r_D_002A4B1C __asm__("D_002A4B1C") = 3.40282347e+38f; }
// If in world: PrepareBombDetonation, send damage/explosion messages to every victim, explode effects, reclaim or
// remove.
// Also known as: EndProjectile (upstream readable / Sly 1 name; EndProjectile__FP3ALO).
extern "C" void DetonateBomb(BOMBV *bomb)
{
    if ((bomb->isInWorld & 1) == 0)
        return;
    PrepareBombDetonation(bomb, GetLoWorldPos(bomb), g_normalZ);
    FRAME frame;
    if (bomb->noBlast == 0) {
        FrameInit(&frame);
        int c;
        SOV **apo;
        int i;
        CollectBombVictims(bomb, &c, &apo);
        for (i = 0; i < c; i++) {
            SOV *po = apo[i];
            ZPR msg;
            U g;
            qword d, n;
            float l;
            func_002333B4(&msg, 0, 0x60);
            msg.zpk = bomb->zapKind;
            msg.dhp = bomb->blastDamage;
            msg.position = bomb->posDetonate;
            msg.ploSource = bomb;
            msg.unknown08 = (LO *)po;
            qword pos = GetLoWorldPos(po);
            qword def = g_normalZ;
            d = VuSub(pos, bomb->posDetonate);
            {
                register qword t __asm__("vf2");
                t = VuDot(d, d);
                __asm__("vsqrt Q, %1x\n\tvwaitq\n\tvaddq.x %0, $vf0, Q" : "=j"(t) : "j"(t));
                l = VuToFloat(t);
                if (l < 1e-4f) {
                    t = def;
                } else {
                    __asm__("vdiv Q, $vf0w, %1x\n\tvwaitq\n\tvmulq.xyzw %0, %2, Q" : "=j"(t) : "j"(t), "j"(d));
                }
                n = t;
            }
            { qword t; t = VuFromFloat<qword>(D_002BC9E0); g.q = t; }
            {
                qword v;
                __asm__("vmulx.xyzw %0, %1, %2x" : "=j"(v) : "j"(n), "j"(g.q));
                msg.velocity = v;
            }
            DeliverZapToSo(&msg);
        }
        FrameFree(&frame, 2);
    }
    bomb->TranslateToPos(bomb->posDetonate);
    bomb->RotateToMat((MATRIX3 *)&bomb->matDetonate);   /* a 3x3 rotation (M3) */
    bomb->SetVelocity(bomb->velocityAtDetonation);
    bomb->SetAngularVelocity(bomb->angularVelocityAtDetonation);
    if (bomb->blastListener) {
        MSG2 msg2;
        POV *po = bomb->blastListener;
        msg2.k = 9;
        msg2.g8 = bomb->blastListenerArg;
        msg2.gC = bomb->sRadius;
        msg2.p = bomb;
        ((void (*)(POV *, MSG2 *))po->pvt[51].pfn)((POV *)((char *)po + po->pvt[51].d), &msg2);
    }
    if (bomb->makesNoise) {
        POV *po = (POV *)bomb->thrower;
        void *pv = bomb;
        if (po != (POV *)bomb && IsBasicDerivedFrom__FP5BASIC3CID(po, 0x70) && po->pp14) {
            POV *pT = *po->pp14;
            if (pT != po) pv = pT;
        }
        NewNoise(g_sw.apstimk[3], GetLoWorldPos(bomb), pv)[0x54 / 4] = 1;
    }
    bomb->Remove();
    bomb->isDetonationPrepared = 0;
    bomb->tDetonate = D_002A4B1C;
    bomb->HandleSpliceEvent(5, 0, 0, 0, 0);
}

namespace RO_ORDER { extern const float r_D_002A4B20 __asm__("D_002A4B20") = 3.40282347e+38f; }
// BOMB vtable slot 26: base, clear detonation, tDetonate=const, thrower=self, unlock constraints, unparent. (inferred)
// Also known as: ResetProjectileInstance (upstream readable / Sly 1 name; ResetProjectileInstance__FP2LO).
extern "C" void ResetBomb(BOMBV *bomb) {
    SO_ResetForProxy(bomb);
    bomb->isDetonationPrepared = 0;
    bomb->tDetonate = D_002A4B20;
    SetBombOwner(bomb, bomb);
    SetSoCnstrForce(bomb, 0);
    SetSoCnstrTorque(bomb, 0);
    SetSoTreeFlag2(bomb, 0);
    bomb->ploStuckTo = 0;
    SetLoParent(bomb, 0, 0xf);
}

// BOMB vtable slot 61: SO base; ignore (max with 1) objects sharing the thrower's root. (inferred)
// Also known as: OverlapKindProjectile (upstream readable / Sly 1 name; OverlapKindProjectile__FP3ALOT0).
extern "C" int ShouldBombIgnoreIntersection(BOMBV *bomb, SOV *pso)
{
    int r = ShouldSoIgnoreIntersection(bomb, pso);
    if (pso->ploroot->plo == bomb->thrower->root->topObject) {
        r = MaxInt(r, 1);
    }
    return r;
}

// Set the thrower (+0x200) and RefreshSwObjectCollisionFilter if changed. (inferred)
// Also known as: SetProjectileLauncher (upstream readable / Sly 1 name; SetProjectileLauncher__FP3ALOP2LO).
extern "C" void SetBombOwner(BOMBV *bomb, BOMBV *pbombOwner)
{
    if (pbombOwner != bomb->thrower) {
        bomb->thrower = pbombOwner;
        RefreshSwObjectCollisionFilter(&g_sw, bomb);
    }
}
