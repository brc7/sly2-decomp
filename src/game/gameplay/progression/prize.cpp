// prize.cpp -- prizes (DPRIZE : SO and its classes: loot, clue bottles, vaults, medicine...): init and load, the
// prize states and pickup, the loot lists, and the vault. Coins (CID91) and medicine (CID93) are "sprayed" out of
// smashed objects and guards (SprayDprizes); clue bottles (CLUE) record their pickup in the save (afClue bits, 30 per
// level); the vault (VAULT) is the safe whose three-digit combination is dialled with the stick. Entry points: the
// DPRIZE/CID91/CID92/CID93/LOOT/CLUE/CLUEPROXY/VAULT vtables, SprayDprizes, the save queries (IsClueCollected,
// CountCollectedClues, IsVaultOpened) and the Splice setters (AddLootEntry*, TriggerVault, SetDprizeParams).
// TU 0x1C4E50..0x1C8CC0 (61 functions, address order).
//
// Types: the classes from sly2/prize.h (DPRIZE, COIN, MEDICINE, KEY, LOOT/LOOTE, CLUE, CLUEPROXY, VAULT). The
// hand-written per-function files keep their other helpers and views in namespaces named after each function (as
// jt.cpp does), their C++-named definitions as C-linkage definitions of their literal symbols there.
// SLY2_ALIAS: __static_initialization_and_destruction_0=StaticInit_prize
// SLY2_ALIAS: _GLOBAL_$I$*=GlobalConstructors_prize

#include "sly2/basic.h"
#include "sly2/so.h"
#include "sly2/slotheap.h"
#include "sly2/stackscope.h"
#include "sly2/sw.h"
#include "sly2/classes.h"
#include "sly2/prize.h"
#include "sly2/stepplayer.h"
#include "sly2/vu0.h"
#include "sly2/util.h"
#include "sly2/act.h"
#include "sly2/cref.h"
#include "sly2/clock.h"
#include "sly2/game.h"
#include "sly2/save.h"
#include "sly2/sndmgr.h"
#include "sly2/emitter.h"   /* EXPL, EXPLSO: the vanish/collect listeners */
#include "sly2/rip.h"

// 0x1C8B68 StaticInit_prize and GlobalConstructors_prize: g++-generated for the six engine slot pools.
SLY2_INSTANTIATE_SLOT_POOLS()

struct OIDT;
extern "C" {
extern OIDT D_002C0D40[];   /* DPRIZE oid table */
extern OIDT D_002C0EC8[];   /* VAULT oid table (0x18 entries) */
void LoadSoFromBrx(SO *pso, CBinaryInputStream *pbis);
void PostSoLoad(SO *pso);
void *EnsureSoInSwList187(SO *pso);
void SetXfRole(SO *pso, XF *pxf, int n);
void BindLoOidTable(LO *plo, int coid, OIDT *aoidt);
int GetCoinCount(void *pv);
void SetCoinCount(void *pv, int n);
void ClearCurrentLootEntry(LOOT *ploot);
void InitDprize(DPRIZE *pdprize);
XF *FindSoXf(SO *pso, int oid, int n);
int IsVaultOpened();
void SetVaultState(VAULT *pvault, int n);
}

namespace M_InitDprize {
extern float D_002A7710 __asm__("D_002A7710");
void InitSo(DPRIZE *p) __asm__("InitSo");
void SetSoZapResponse(DPRIZE *p, int a, int b) __asm__("SetSoZapResponse");
// DPRIZE::Init (vtable slot 7): InitSo + 0x1F7918 setup.
// Also known as: InitPickup (upstream readable / Sly 1 name; InitPickup__FP2LO).
extern "C" void InitDprize(DPRIZE *prize)
{
    sptr v;
    InitSo(prize);
    SetSoZapResponse(prize, 0x17E, 8);
    v = (sptr)prize->ploupd;
    prize->unknown21C = 30.0f;
    prize->isCollectable = 1;
    prize->expireTime = D_002A7710;
    prize->state = -1;
    prize->initialState = 0;
    prize->idleAnimOverride = -1;
    prize->glintOwnerOid = OID_Nil;
    prize->leftEmitterOid = OID_Nil;
    prize->rightEmitterOid = OID_Nil;
    prize->linkedOid = OID_Nil;
    prize->collectListenerOid = OID_Nil;
    prize->vanishListenerOid = OID_Nil;
    prize->unknown224 = OID_Nil;
    if (IsPointer(v)) ((LOUPD *)v)->grf |= 2; else prize->unknown40 = v | 2;
}
} // namespace M_InitDprize

// DPRIZE::LoadFromBrx (vtable slot 14): LoadSoFromBrx + snip objects (BindLoOidTable).
extern "C" void LoadDprizeFromBrx(DPRIZE *prize, CBinaryInputStream *pbis)
{
    LoadSoFromBrx(prize, pbis);
    *(int *)((char *)EnsureSoInSwList187(prize) + 0xF0) = 1;
    SetXfRole(prize, prize->positionXf, 0x15);
    SetXfRole(prize, prize->rotationXf, 0x15);
    BindLoOidTable(prize, 1, D_002C0D40);
}

namespace M_PostDprizeLoad {
void PostSoLoad(DPRIZE *p) __asm__("PostSoLoad");
LO *FindSwObjectByClass__FP2SWi3CIDP2LO(void *pg, int a, int b, DPRIZE *p) __asm__("FindSwObjectByClass__FP2SWi3CIDP2LO");
LO *FindSwObject__FP2SWi3OIDP2LO(void *pg, int a, int b, DPRIZE *p) __asm__("FindSwObject__FP2SWi3OIDP2LO");
// DPRIZE vtable slot 15 PostLoad: base + resolve linked objects via SW lookup.
// Also known as: PostLoadPickup (upstream readable / Sly 1 name; PostLoadPickup__FP2LO).
extern "C" void PostDprizeLoad(DPRIZE *prize)
{
    LO *plo;
    PostSoLoad(prize);
    plo = FindSwObjectByClass__FP2SWi3CIDP2LO(&g_sw, 1, 0x12, prize);
    prize->target = plo;
    if (plo)
        plo->Remove();
    if (prize->glintOwnerOid != -1) prize->glintOwner = FindSwObject__FP2SWi3OIDP2LO(&g_sw, 4, prize->glintOwnerOid, prize);
    if (prize->leftEmitterOid != -1) prize->leftEmitter = FindSwObject__FP2SWi3OIDP2LO(&g_sw, 4, prize->leftEmitterOid, prize);
    if (prize->rightEmitterOid != -1) prize->rightEmitter = FindSwObject__FP2SWi3OIDP2LO(&g_sw, 4, prize->rightEmitterOid, prize);
    if (prize->linkedOid != -1) prize->linked = FindSwObject__FP2SWi3OIDP2LO(&g_sw, 4, prize->linkedOid, prize);
    if (prize->collectListenerOid != -1) prize->collectListener = FindSwObject__FP2SWi3OIDP2LO(&g_sw, 4, prize->collectListenerOid, prize);
    if (prize->vanishListenerOid != -1) prize->vanishListener = FindSwObject__FP2SWi3OIDP2LO(&g_sw, 4, prize->vanishListenerOid, prize);
    prize->SetState(prize->initialState);
}
} // namespace M_PostDprizeLoad

namespace M_HandleDprizeMessage {
   /* the vanish listener: an EXPL */
struct A { int c; void *pv; int pad[14]; };
struct M { SMA *sender; int x4; int goalAnim; };
void HandleBasicMessage(DPRIZE *p, int n, struct M *pm) __asm__("HandleBasicMessage");
// DPRIZE vtable slot 5 HandleMessage: base HandleBasicMessage + prize-specific msgs. (inferred)
// Also known as: OnPickupMessage (upstream readable / Sly 1 name; OnPickupMessage__FP2LO5MSGIDPv).
extern "C" void HandleDprizeMessage(DPRIZE *prize, int n, struct M *pm)
{
    struct A a;
    HandleBasicMessage(prize, n, pm);
    if (n == 10 && pm->sender == prize->sma && pm->goalAnim == 0x186) {
        struct EXPL *plo = (struct EXPL *)prize->vanishListener;
        if (plo) {
            a.c = 1;
            a.pv = prize;
            ((EXPL *)plo)->ExplodeExplso((EXPLSO *)&a);   /* A: EXPLSO grf 1 and ploReference */
        }
        prize->SetState(2);
    }
}
} // namespace M_HandleDprizeMessage

namespace M_ProjectDprizeTransform {
// ProjectDprizeTransform (w2lane4, wave 2): per-frame update of an animated object (state 6 only forwards to UpdateSoXfWorld).
// Step 1: damped velocity/position integration of the object's own coordinate (VU: velocity = velocity * damping + acceleration * dt,
//         coordinate += velocity * dt) which is pushed into the position channel.
// Step 2: if the prize's state machine (SMA) has a current animation, find the position/orientation motions of this object in the animation's action
//         (entries keyed by id + channel via virtual slot 51), sample them (virtual slot 3) and apply them in the frame that
//         BuildOrthonormalMatrixZ builds from (camera position - channel position, z = 0).
// Matching notes:
//  * "rV" on the dt word keeps the load as an asm memory operand (qmtc2 source gets its own reload register, which decides
//    v0/v1 and the lqc2/qmtc2 order of the first VU block).
//  * The locals sit in the innermost block so that the VU_FLOAT temporaries share the first stack slot (sp+0) and the
//    positions/frame follow it; BuildOrthonormalMatrixZ/LoadRotateMatrixEuler return M3 by value (hidden pointer) and are assigned to locals.
//  * operator* is the usual inline matrix product with a named return value plus a dead local copy `t`; each row is stored to
//    `t` and `mat` right after its asm (the order of those stores decides the vf numbering of the loaded rows).
//  * `spin` is a second variable for p->orientation so that `current` and the later reload get different registers.
union U { qword q; float f[4]; int words[4]; };
union M3 { qword rows[3]; float values[12]; };
/* The prize's two XFs (SO unknown58 / unknown5C: the position and orientation channels) as this function moves them:
   the local rotation rows, the local position (row 3), the world stamp and the velocity as quadwords. */
struct CHANNEL { M3 matrix; qword position; char pad40[0x64]; int stamp; char padA8[8]; qword velocity; };
   /* CLOCK island: deltaTime read as float and as int bits (FLOATWORD) */

extern float D_002C0D4C __asm__("D_002C0D4C");
extern qword g_normalZ __asm__("g_normalZ");
void UpdateSoXfWorld(DPRIZE *) __asm__("UpdateSoXfWorld");
qword *VECTOR_opAssign(qword *, qword) __asm__("VECTOR_opAssign");
M3 BuildOrthonormalMatrixZ(qword, qword) __asm__("BuildOrthonormalMatrixZ");
M3 LoadRotateMatrixEuler(qword) __asm__("LoadRotateMatrixEuler");


struct VU_FLOAT {
    qword q;
    // Broadcast a float into a VU0 register.
    VU_FLOAT(float f) { __asm__("qmtc2.ni %1, %0" : "=j"(q) : "r"(f)); }
    // Broadcast int bits into a VU0 register.
    VU_FLOAT(int bits) { __asm__("qmtc2.ni %1, %0" : "=j"(q) : "rV"(bits)); }
};
struct VU_VECTOR {
    qword q;
    // Wrap a qword as a VU_VECTOR.
    VU_VECTOR(const qword &v) { q = v; }
};
// Weighted sum f1*v1 + f2*v2.
static inline qword VecCombo(VU_FLOAT f1, VU_VECTOR v1, VU_FLOAT f2, VU_VECTOR v2)
{
    qword r;
    __asm__("vmulax.xyzw $ACC, %2, %1x\n\tvmaddx.xyzw %0, %4, %3x" : "=j"(r) : "j"(f1.q), "j"(v1.q), "j"(f2.q), "j"(v2.q));
    return r;
}
// value + velocity * time.
static inline qword Advance(qword value, qword velocity, VU_FLOAT time)
{
    qword r;
    __asm__("vadda.xyzw $ACC, %1, $vf0\n\tvmaddx.xyzw %0, %2, %3x" : "=j"(r) : "j"(value), "j"(velocity), "j"(time.q));
    return r;
}
// Vector difference.
// Vector sum.
// 3x3 matrix times vector.
static inline qword RotateVec(const M3 &m, qword value)
{
    __asm__("vmulax.xyz $ACC, %1, %0x\n\tvmadday.xyz $ACC, %2, %0y\n\tvmaddz.xyz %0, %3, %0z"
            : "+j"(value) : "j"(m.rows[0]), "j"(m.rows[1]), "j"(m.rows[2]));
    return value;
}
// 3x3 matrix product matL * matR.
static inline M3 operator*(const M3 &matL, const M3 &matR) return mat
{
    M3 t;
    qword qwL0 = matL.rows[0], qwL1 = matL.rows[1], qwL2 = matL.rows[2];
    qword v0, v1, v2;
    __asm__("vmulax.xyz $ACC, %1, %4x\n\tvmadday.xyz $ACC, %2, %4y\n\tvmaddz.xyz %0, %3, %4z"
            : "=j"(v0) : "j"(qwL0), "j"(qwL1), "j"(qwL2), "j"(matR.rows[0]));
    t.rows[0] = v0; mat.rows[0] = v0;
    __asm__("vmulax.xyz $ACC, %1, %4x\n\tvmadday.xyz $ACC, %2, %4y\n\tvmaddz.xyz %0, %3, %4z"
            : "=j"(v1) : "j"(qwL0), "j"(qwL1), "j"(qwL2), "j"(matR.rows[1]));
    t.rows[1] = v1; mat.rows[1] = v1;
    __asm__("vmulax.xyz $ACC, %1, %4x\n\tvmadday.xyz $ACC, %2, %4y\n\tvmaddz.xyz %0, %3, %4z"
            : "=j"(v2) : "j"(qwL0), "j"(qwL1), "j"(qwL2), "j"(matR.rows[2]));
    t.rows[2] = v2; mat.rows[2] = v2;
}
// Motion channel of the prize (vtable slot 51).
static inline CHANNEL *GetChannel(DPRIZE *prize, int channel)
{
    return (CHANNEL *)prize->GetXfFromIndex(channel);
}
// Samples a motion curve at the animation time: value and velocity (motion vtable slot 3).
static inline void Sample(AC *motion, const ASEGA *animation, qword *value, qword *velocity)
{
    motion->Evaluate(animation->time, animation->speed, value, velocity);
}

// DPRIZE vtable slot 17: damped spring/hover integration of the prize position (state 6 -> base). (inferred)
// Also known as: UpdatePickupMotion (upstream readable / Sly 1 name; UpdatePickupMotion__FP2LO).
extern "C" void ProjectDprizeTransform(DPRIZE *prize)
{
    if (prize->state == 6) { UpdateSoXfWorld(prize); return; }
    float factor;
    if (prize->state == 0 || prize->state == 5) factor = 1.0f + D_002C0D4C * g_clock.deltaTime;
    else factor = 1.0f;
    prize->velocity = VecCombo(VU_FLOAT(factor), VU_VECTOR(prize->velocity), VU_FLOAT(g_clock.deltaTimeBits), VU_VECTOR(prize->acceleration));
    qword newPosition = Advance(prize->position, prize->velocity, VU_FLOAT(g_clock.deltaTimeBits));
    CHANNEL *channel = (CHANNEL *)prize->positionXf;
    prize->position = newPosition;
    VECTOR_opAssign(&channel->position, newPosition);
    *(float *)((char *)channel + 0x3C) = 1.0f;
    ((CHANNEL *)prize->positionXf)->velocity = prize->velocity;
    { int stamp = ++g_sw.transformStamp; ((CHANNEL *)prize->positionXf)->stamp = stamp; }
    if (prize->sma && prize->sma->pasegaCur) {
        ASEGA *animation = prize->sma->pasegaCur;
        ASEG *action = animation->animation;
        AC *positionMotion = 0, *orientationMotion = 0;
        int i = 0;
        for (; i < action->cchn; i++) {
            CHN *entry = &action->achn[i];
            unsigned int key = entry->grf;
            if (!(key & 0x40000000U)) continue;
            if (((int)(key << 12) >> 12) != prize->oid) continue;
            {
                CHANNEL *selected = GetChannel(prize, (int)(key << 2) >> 22);
                if (selected == (CHANNEL *)prize->positionXf) positionMotion = entry->pac;
                else if (selected == (CHANNEL *)prize->rotationXf) orientationMotion = entry->pac;
            }
            if (positionMotion && orientationMotion) break;
        }
        if (positionMotion || orientationMotion) {
            U dir, position, velocity, angles, angularVelocity;
            M3 frame, rot;
            dir.q = VuSub<qword>(g_cm.matq.pos, ((CHANNEL *)prize->positionXf)->position);
            dir.words[2] = 0;
            frame = BuildOrthonormalMatrixZ(dir.q, g_normalZ);
            if (positionMotion) {
                Sample(positionMotion, animation, &position.q, &velocity.q);
                channel = (CHANNEL *)prize->positionXf;
                VECTOR_opAssign(&channel->position, VuAdd(channel->position, RotateVec(frame, position.q)));
                *(float *)((char *)channel + 0x3C) = 1.0f;
                CHANNEL *current = (CHANNEL *)prize->positionXf;
                current->velocity = VuAdd(current->velocity, RotateVec(frame, velocity.q));
                { int stamp = ++g_sw.transformStamp; ((CHANNEL *)prize->positionXf)->stamp = stamp; }
            }
            if (orientationMotion) {
                Sample(orientationMotion, animation, &angles.q, &angularVelocity.q);
                rot = LoadRotateMatrixEuler(angles.q);
                CHANNEL *current = (CHANNEL *)prize->rotationXf;
                current->matrix = frame * rot;
                current->matrix.values[3] = 0.0f; current->matrix.values[7] = 0.0f; current->matrix.values[11] = 0.0f;
                CHANNEL *spin = (CHANNEL *)prize->rotationXf;
                spin->velocity = RotateVec(frame, angularVelocity.q);
                { int stamp = ++g_sw.transformStamp; ((CHANNEL *)prize->rotationXf)->stamp = stamp; }
            }
        }
    }
    UpdateSoXfWorld(prize);
}
} // namespace M_ProjectDprizeTransform

namespace M_IsDprizeCollisionCandidate {
struct C { char pad[0x60]; void *p60; };
struct B { char pad[0xEC]; struct C *pc; };
struct Q { int a; struct B *pb; int pad[2]; unsigned long fl; };
int IsBasicDerivedFrom__FP5BASIC3CID(void *p, int n) __asm__("IsBasicDerivedFrom__FP5BASIC3CID");
// collision filter callback (FIsCidDerived test). (inferred)
extern "C" int IsDprizeCollisionCandidate(void *prize, struct Q *q)
{
    void *s;
    /*B*/if (q->fl & (1UL << 48)) return 0; s = q->pb->pc->p60; if (IsBasicDerivedFrom__FP5BASIC3CID(s, 0x32) || IsBasicDerivedFrom__FP5BASIC3CID(s, 0x4F)) return 0; return 1;/*E*/
}
} // namespace M_IsDprizeCollisionCandidate

namespace M_CheckDprizeBounce {
// CheckDprizeBounce: one commented pin (QwNormalizeInto), one allocation-only asm operand (GFromVf), VU one-op asm helpers only.
// dprize-style collision response (next to matched TryCollectDprize): query the radius box with IntersectSwBoundingBox, find the deepest
// sphere / CplaneFromSphere contact that the velocity approaches, then push the position out and rebuild the velocity (floor case:
// tangent + clamped normal parts, one-shot boost; otherwise normal/tangent split with the two response factors).
// Keys (wave 4, from w3lane8.v3 at aligned 52): call results passed raw so gcc emits the qmtc2; QwNormalizeInto reads the vector
// like matched ProjectLeafTransform's NormalizeSafe; GDot's 1.0 temp is not early-clobber; "rV" on every qmtc2 of a float word read from
// memory (the lw becomes a reload: frees local-alloc and resets reload's round-robin); gSpeed read with the mtc1 form.
// Last fix (wave 5): the length test also reads its float with the mtc1 asm. A reload mtc1 has latency 2, which gave the
// gNormal copy (anti dependence on that $f0 write) sched2 priority 7 > 6 for the li.s 1e-4; the asm mtc1 costs 1, so the
// two tie and the li.s goes first as in the target.
union U { qword qw; float g[4]; };

struct SOPART { XF *transform; void *owner; float sRadius; int oid; unsigned long grf; char pad18[0x5C]; int cplane; };
struct CMCONTACT { qword pos; U plane; int kind; void *pv; };


extern qword D_002A2DA0 __asm__("D_002A2DA0"), g_normalZ __asm__("g_normalZ"), D_002A2DD0 __asm__("D_002A2DD0"), D_002A2DE0 __asm__("D_002A2DE0");
extern float D_002C0D74 __asm__("D_002C0D74"), D_002C0D78 __asm__("D_002C0D78");
void STACKSCOPE_ctor(STACKSCOPE *) __asm__("STACKSCOPE_ctor");
void STACKSCOPE_ctor_1AE4A0(STACKSCOPE *, int) __asm__("_$_10STACKSCOPE");
qword GetLoWorldPos(DPRIZE *) __asm__("GetLoWorldPos");
int IsDprizeCollisionCandidate(void *, SOPART *) __asm__("IsDprizeCollisionCandidate");
void IntersectSwBoundingBox(void *, qword, qword, int (*)(void *, SOPART *), void *, int *, SOPART ***) __asm__("IntersectSwBoundingBox");
int IntersectSphereBsp(qword, float, SOPART *, void *, int, void *, int, CMCONTACT *) __asm__("IntersectSphereBsp");
float SplitVectorAlongAxis(qword, qword, qword *, qword *) __asm__("SplitVectorAlongAxis");


struct VU_FLOAT {
    qword qw;
    // Broadcast a float into a VU0 register.
    VU_FLOAT(float g) { __asm__("qmtc2.ni %1, %0" : "=j"(qw) : "r"(g)); }
    // Broadcast int bits into a VU0 register.
    VU_FLOAT(int bits) { __asm__("qmtc2.ni %1, %0" : "=j"(qw) : "rV"(bits)); }
};
// Helpers VuFloat, VuFloatBits, GFromQw (small asm wrappers/accessors shaped for the match).
static inline qword VuFloatBits(int g) { qword r; __asm__("qmtc2.ni %1, %0" : "=j"(r) : "rV"(g)); return r; }

/* the second "j"(q) is allocation-only: it keeps the length qword in a VU register (the "r" use alone ties the class
   costs and puts it in a GPR), so gcc reloads it with its own qmfc2 into v1 like the target. */
static inline float GFromVf(qword q) { float g; __asm__("mtc1 %1, %0" : "=f"(g) : "r"(q), "j"(q)); return g; }
// base + vec * scale.
static inline qword QwAddScaled(qword base, qword dir, qword g)
{
    qword r;
    __asm__("vadda.xyzw $ACC, %1, $vf0\n\tvmaddx.xyzw %0, %2, %3x" : "=j"(r) : "j"(base), "j"(dir), "j"(g));
    return r;
}
// Weighted sum of two vectors.
static inline qword QwCombo(VU_FLOAT g1, qword v1, VU_FLOAT g2, qword v2)
{
    qword r;
    __asm__("vmulax.xyzw $ACC, %2, %1x\n\tvmaddx.xyzw %0, %4, %3x" : "=j"(r) : "j"(g1.qw), "j"(v1), "j"(g2.qw), "j"(v2));
    return r;
}
// Vector difference.
// Length of a vector, in lane x.
static inline qword QwLength(qword v)
{
    qword d, one, len;
    __asm__("vmul.xyz %0, %2, %2\n\tvaddw.x %1, $vf0, $vf0w\n\tvadday.x $ACC, %0, %0y\n\tvmaddz.x %0, %1, %0z" : "=j"(d), "=&j"(one) : "j"(v));
    __asm__("vsqrt $Q, %1x\n\tvwaitq\n\tvaddq.x %0, $vf0, $Q" : "=j"(len) : "j"(d));
    return len;
}
// Vector divided by its length.
// Unit vector (or fallback for a near-zero vector).
static inline qword QwNormalize(qword v, qword fallback)
{
    qword len = QwLength(v);
    if (VuToFloat(len) < 0.0001f)
        return fallback;
    return VuDivByX(v, len);
}
// Unit vector stored through a pointer.
static inline qword QwNormalizeInto(qword *pv, const qword &fallback)
{
    register qword g asm("$2") = *(volatile qword *)pv; /* the one pin (as in ProjectLeafTransform): the target reads the vector into $v0, then qmtc2 */
    qword v = g;
    qword len = QwLength(v);
    if (GFromVf(len) < 0.0001f)
        *pv = fallback;
    else
        *pv = VuDivByX(v, len);
    return len;
}
// Dot product as a float.
static inline float GDot(qword a, qword b)
{
    qword d, one;
    __asm__("vmul.xyz %0, %2, %3\n\tvaddw.x %1, $vf0, $vf0w\n\tvadday.x $ACC, %0, %0y\n\tvmaddz.x %0, %1, %0z" : "=j"(d), "=j"(one) : "j"(a), "j"(b));
    return VuToFloat(d);
}
// The smaller of two floats.

// query nearby solids (IntersectSwBoundingBox) and resolve deepest contact (bounce). (inferred)
// Also known as: UpdatePickupGround (upstream readable / Sly 1 name; UpdatePickupGround__FP2LO).
extern "C" void CheckDprizeBounce(DPRIZE *prize)
{
    STACKSCOPE frame;
    U vec;
    float gDeepest = 0.0f;
    qword pos = GetLoWorldPos(prize);
    qword posMin = (vec.qw = VuFromFloat<qword>(-prize->bounceRadius), QwAddScaled(pos, D_002A2DE0, vec.qw));
    qword posMax = GetLoWorldPos(prize);
    vec.qw = VuFloatBits(*(int *)&prize->bounceRadius);
    posMax = QwAddScaled(posMax, D_002A2DE0, vec.qw);
    int cpart;
    SOPART **appart;
    IntersectSwBoundingBox(&g_sw, posMin, posMax, IsDprizeCollisionCandidate, 0, &cpart, &appart);
    U normal;
    CMCONTACT acontact[8];
    for (int i = 0; i < cpart; i++) {
        SOPART *ppart = appart[i];
        if (ppart->grf & (1UL << 49)) {
            qword center = ppart->transform->worldMatq.r[3].q;
            qword posCur = GetLoWorldPos(prize);
            float g = VuToFloat(QwLength(VuSub(center, posCur))) - ppart->sRadius - prize->bounceRadius;
            if (g < gDeepest) {
                vec.qw = QwNormalize(VuSub<qword>(prize->position, ppart->transform->worldMatq.r[3].q), D_002A2DA0);
                if (GDot(prize->velocity, vec.qw) < 0.0f) {
                    gDeepest = g;
                    normal.qw = vec.qw;
                }
            }
        } else if (ppart->cplane) {
            int ccontact = IntersectSphereBsp(GetLoWorldPos(prize), prize->bounceRadius, ppart, (void *)SLY2_BSP_NO_LIMIT, 0, 0, 8, acontact);
            for (int j = 0; j < ccontact; j++) {
                CMCONTACT *pcontact = &acontact[j];
                float g = GDot(pcontact->pos, pcontact->plane.qw) - pcontact->plane.g[3];
                if (g < gDeepest && GDot(prize->velocity, pcontact->plane.qw) < 0.0f) {
                    normal.qw = pcontact->plane.qw;
                    gDeepest = g;
                }
            }
        }
    }
    if (gDeepest < 0.0f) {
        U response;
        {
            U primary;
            if (normal.g[2] > 0.7f && prize->state == 4) {
                float gNormal = SplitVectorAlongAxis(prize->velocity, g_normalZ, 0, &primary.qw);
                float gSpeed = GFromVf(QwNormalizeInto(&primary.qw, D_002A2DD0));
                float gTangent = StdMin(prize->tangentResponse * gSpeed, D_002C0D74);
                gNormal = StdMin(-prize->normalResponse * gNormal, D_002C0D78);
                response.qw = QwCombo(VU_FLOAT(gTangent), primary.qw, VU_FLOAT(gNormal), g_normalZ);
                if (!prize->hasBoosted) {
                    int fTrigger = GDot(response.qw, normal.qw) < prize->bounceBoostThreshold;
                    if (fTrigger) {
                        response.qw = QwCombo(VU_FLOAT(gTangent), primary.qw, VU_FLOAT(*(int *)&prize->bounceBoost), g_normalZ);
                        prize->hasBoosted = 1;
                    }
                }
            } else {
                U secondary;
                SplitVectorAlongAxis(prize->velocity, normal.qw, &primary.qw, &secondary.qw);
                response.qw = QwCombo(VU_FLOAT(-prize->normalResponse), primary.qw, VU_FLOAT(*(int *)&prize->tangentResponse), secondary.qw);
            }
        }
        prize->position = QwAddScaled(prize->position, normal.qw, VU_FLOAT(-gDeepest).qw);
        prize->velocity = response.qw;
    }
}
} // namespace M_CheckDprizeBounce

namespace M_FCheckDprizeCollect {
struct Q { int enabled,pad4; float radius; SO *so; };
extern float D_002C0D7C __asm__("D_002C0D7C");
qword GetLoWorldPos(void *) __asm__("GetLoWorldPos");
int IntersectSphereBsp(qword,SOPART *,uptr mask,int triangleCount,void *triangles,int capacity,void *contacts,float radius) __asm__("IntersectSphereBsp");
// Sets the prize to state 1 through vtable slot 76 (SetDprizes); used when a zap collects it. (inferred)
extern "C" { static inline void Trigger(DPRIZE *p)
{
    p->SetState(1);
} }
// Moves a 128-bit EE register into a VU register.
extern "C" { static inline qword ToVu(qword a)
{
    qword b;
    __asm__("qmtc2 %1,%0":"=j"(b):"r"(a));
    return b;
} }
// Vector difference.
// Squared length of a vector.
extern "C" { static inline float LenSq(qword a)
{
    qword d,one;
    float f;
    __asm__("vmul.xyz %0,%2,%2\n\tvaddw.x %1,$vf0,$vf0w\n\tvadday.x $ACC,%0,%0y\n\tvmaddz.x %0,%1,%0z":"=j"(d),"=&j"(one):"j"(a));
    __asm__("mtc1 %1,%0":"=f"(f):"r"(d));
    return f;
} }
// test one collector (PZI) against the prize: in range -> collect. (inferred)
// Also known as: FPickupTouched (upstream readable / Sly 1 name; FPickupTouched__FP2LOP6PICKUP).
extern "C" int TryCollectDprize(DPRIZE *prize,struct Q *q)
{
    char scratch[0x30] __attribute__((aligned(16)));
    qword pos;
    int i;
    float distance,maximum;
    if(!q->enabled||!q->so)return 0;
    pos=ToVu(GetLoWorldPos(prize));
    distance=LenSq(VuSub(pos,GetLoWorldPos(q->so)));
    maximum=D_002C0D7C;
    if(maximum*maximum<distance)return 0;
    if(distance<q->radius*q->radius) {
        Trigger(prize);
        return 1;
    }
    for(i=0;i<q->so->numParts;i++) {
        SOPART *s=&q->so->parts[i];
        if(s->isHidden)continue;
        if(s->unknownFlag10B) {
            float r=prize->collectRadius+s->radius;
            qword here=GetLoWorldPos(prize);
            if(LenSq(VuSub(here,*(qword *)s->transform->worldMatrix.mat[3]))<r*r) {
                Trigger(prize);
                return 1;
            }
        } else {
            qword here=GetLoWorldPos(prize);
            if(IntersectSphereBsp(here,s,SLY2_BSP_NO_LIMIT,0,0,1,scratch,prize->collectRadius)) {
                Trigger(prize);
                return 1;
            }
        }
    }
    return 0;
}

#define OFF(t,m) ((unsigned int)&((t*)0)->m)
typedef char trigger_layout[(sizeof(void*)==4 && sizeof(qword)==16 && __alignof__(qword)==16 && sizeof(unsigned long)==8 && sizeof(struct Q)==0x10 && OFF(struct Q,enabled)==0 && OFF(struct Q,radius)==8 && OFF(struct Q,so)==0xC)?1:-1];
} // namespace M_FCheckDprizeCollect
#undef OFF

namespace M_CheckDprizeCollect {
int CollectActivePickupInfos(void *pg, int c, void **ap) __asm__("CollectActivePickupInfos");
int IsBasicDerivedFrom__FP5BASIC3CID(DPRIZE *p, int n) __asm__("IsBasicDerivedFrom__FP5BASIC3CID");
int TryCollectDprize(DPRIZE *p, void *pv) __asm__("TryCollectDprize");
// for each collector PZI (CppziCur) check collection. (inferred)
// Also known as: TestPickupCollect (upstream readable / Sly 1 name; TestPickupCollect__FP2LO).
extern "C" void CheckDprizeCollect(DPRIZE *prize)
{
    void *ap[0x10];
    int i, n;
    if (!prize->isCollectable) return;
    if (prize->expireTime < g_clock.time) {
        prize->SetState(1);
        return;
    }
    n = CollectActivePickupInfos(&g_sw, 0x10, ap);
    for (i = 0; ; i++) {
        if (i >= n) break;
        if (i > 0 && !IsBasicDerivedFrom__FP5BASIC3CID(prize, 0x5B)) break;
        if (TryCollectDprize(prize, ap[i])) break;
    }
}
} // namespace M_CheckDprizeCollect

// DPRIZE::TakeDamage: a collectable prize hit by a zap with flag 8 counts as collected.
int DPRIZE::TakeDamage(ZPR *pzpr)   /* key function: DPRIZE's vtable comes out here */
{
    if (!isCollectable) return 0;
    if (!(pzpr->grfzpr & 8)) return 0;
    SetState(1);
    return 1;
}

// .rodata order (docs/guides/tu-merging.md "Phase 5: data order"): a named constant is emitted where it is
// defined, a function's literals and jump tables with the function, so each constant is defined where the ELF
// has it among the functions, under an asm label (no function sees a definition under the name it reads).
namespace RO_ORDER { extern const float r_D_002A7710 __asm__("D_002A7710") = 3.40282347e+38f; }
namespace M_UpdateDprize {

extern float D_002C0D50 __asm__("D_002C0D50");


void UpdateSo(DPRIZE *p) __asm__("UpdateSo");
void EmitDprizeGlint(DPRIZE *p) __asm__("EmitDprizeGlint");
void CheckDprizeBounce(DPRIZE *p) __asm__("CheckDprizeBounce");
void CheckDprizeCollect(DPRIZE *p) __asm__("CheckDprizeCollect");
void UpdateDprize(DPRIZE *p) __asm__("UpdateDprize");

// Squared length of a vector.
static inline float LenSq(qword v)
{
    float r;
    qword t, one;
    __asm__("vmul.xyz %0, %2, %2\n\tvaddw.x %1, $vf0, $vf0w\n\tvadday.x $ACC, %0, %0y\n\tvmaddz.x %0, %1, %0z" : "=j"(t), "=&j"(one) : "j"(v));
    __asm__("qmfc2.ni %0, %1" : "=r"(r) : "j"(t));
    return r;
}
// DPRIZE vtable slot 16 Update: base update, sparkle (EmitDprizeGlint), bounce, collect.
// Also known as: UpdatePickupBase (upstream readable / Sly 1 name; UpdatePickupBase__FP2LO).
void UpdateDprize(DPRIZE *prize)
{
    UpdateSo(prize);
    EmitDprizeGlint(prize);
    switch (prize->state) {
    case DPRIZE_STATE_IDLE:
        if (LenSq(prize->velocity) > 10000.0f)
            CheckDprizeBounce(prize);
    case DPRIZE_STATE_VANISHING:
        CheckDprizeCollect(prize);
        break;
    case DPRIZE_STATE_LAUNCHED:
        if (*(float *)((char *)&prize->velocity + 8) < 0.0f)
            prize->SetState(DPRIZE_STATE_FALLING);
        break;
    case DPRIZE_STATE_FALLING:
        if (prize->hasBoosted && *(float *)((char *)&prize->velocity + 8) < 25.0f) {
            prize->hasBoosted = 0;
            prize->SetState(DPRIZE_STATE_IDLE);
        } else if (D_002C0D50 < g_clock.time - prize->stateTime) {
            prize->SetState(DPRIZE_STATE_GONE);
        } else {
            CheckDprizeBounce(prize);
            CheckDprizeCollect(prize);
        }
        break;
    case DPRIZE_STATE_COLLECTED:
    case DPRIZE_STATE_GONE:
    case DPRIZE_STATE_HELD:
        break;
    }
}
} // namespace M_UpdateDprize

namespace M_RenderDprizeSelf {
struct M4 { qword a, b, c, d; };
struct V { float x, y, z, w; } __attribute__((aligned(16)));

CHN *FindAsegChannel(void *p, DPRIZE *plo, int a, int b) __asm__("FindAsegChannel");
qword GetLoWorldPos(DPRIZE *plo) __asm__("GetLoWorldPos");
void *GetLoWorldMatrix(DPRIZE *plo) __asm__("GetLoWorldMatrix");
void RenderSoSelf(DPRIZE *plo) __asm__("RenderSoSelf");
void RenderDprizeSelf(DPRIZE *p) __asm__("RenderDprizeSelf");

M4 LoadMatrixFromPosRotScale(qword q, void *pv, V *pvec) __asm__("LoadMatrixFromPosRotScale");
void RenderLoAtMatrix(DPRIZE *plo, M4 m, float a, float b) __asm__("RenderLoAtMatrix");
// DPRIZE vtable slot 19 RenderSelf (bob/spin then SO render).
void RenderDprizeSelf(DPRIZE *prize)
{
    ASEGA *q;
    CHN *ph;
    if (prize->state == 2) return;
    if (prize->sma && (q = prize->sma->pasegaCur) != 0) {
        ph = FindAsegChannel(q->animation, prize, 0xC, 1);
        if (ph) {
            V v;
            ph->pac->Evaluate(q->time, q->speed, &v, 0);
            M4 m;
            m = LoadMatrixFromPosRotScale(GetLoWorldPos(prize), GetLoWorldMatrix(prize), &v);
            RenderLoAtMatrix(prize, m, prize->opacity, prize->opacity);
        } else {
            RenderSoSelf(prize);
        }
    } else {
        RenderSoSelf(prize);
    }
}
} // namespace M_RenderDprizeSelf

namespace RO_ORDER { extern const float r_D_002A773C __asm__("D_002A773C") = 3.40282347e+38f; }
namespace M_SetDprizeDprizes {
/* The position XF as quadwords (local translation +0x30, velocity +0xB0). */

extern qword D_002A2DD0 __asm__("D_002A2DD0");
extern qword D_002C0D60 __asm__("D_002C0D60");
extern float D_002A773C __asm__("D_002A773C");
void SetLoParent(DPRIZE *p, int a, int b) __asm__("SetLoParent");
LO *GetProxyCopyValue(DPRIZE *p) __asm__("GetProxyCopyValue");
void UnlockProxyObject(LO *q, DPRIZE *p) __asm__("UnlockProxyObject");
SMA *ApplySmToObject(SM *a, DPRIZE *p, int anim, int z) __asm__("ApplySmToObject");
void SkipSma(SMA *q, float g) __asm__("SkipSma");
void SetSmaGoal(SMA *q, int anim) __asm__("SetSmaGoal");
void RetractSma(SMA *q) __asm__("RetractSma");
// DPRIZE state machine setter (unlock proxy copy, attach/detach, state timestamps).
// Also known as: SetPickupStateBase (upstream readable / Sly 1 name; SetPickupStateBase__FP2LOi).
extern "C" void SetDprizeDprizes(DPRIZE *prize, int n)
{
    int anim = -1;
    LO *q;
    if (prize->state == n) return;
    if (n != DPRIZE_STATE_IDLE && n != DPRIZE_STATE_VANISHING)
        SetLoParent(prize, 0, 0);
    if (prize->state == DPRIZE_STATE_NONE || prize->state == DPRIZE_STATE_GONE) {
        struct XF *o = (struct XF *)prize->positionXf;
        prize->position = o->localMatq.r[3].q;
        prize->velocity = o->velq.q;
    }
    switch (n) {
    case DPRIZE_STATE_IDLE:
        anim = prize->idleAnimOverride != -1 ? prize->idleAnimOverride : 0x184;
        prize->acceleration = D_002A2DD0;
        break;
    case DPRIZE_STATE_LAUNCHED:
    case DPRIZE_STATE_FALLING:
        anim = 0x183;
        prize->acceleration = D_002C0D60;
        break;
    case DPRIZE_STATE_GONE:
        prize->expireTime = D_002A773C;
        prize->Remove();
        {
            LO *r = GetProxyCopyValue(prize);
            if (r)
                UnlockProxyObject(r, prize);
        }
        break;
    case DPRIZE_STATE_VANISHING:
        anim = 0x186;
        prize->acceleration = D_002A2DD0;
        break;
    }
    /* the lock-on TARGET child exists only while the prize can be picked up (vtable slots 8/9: Add/Remove) */
    switch (n) {
    case DPRIZE_STATE_IDLE:
    case DPRIZE_STATE_FALLING:
    case DPRIZE_STATE_VANISHING:
        q = prize->target;
        if (q) q->Add();
        break;
    default:
        q = prize->target;
        if (q) q->Remove();
        break;
    }
    prize->state = n;
    prize->stateTime = g_clock.time;
    if (anim != -1) {
        if (!prize->sma) {
            prize->sma = ApplySmToObject(prize->smaSource, prize, anim, 0);
            prize->sma->AddRef();
            if (prize->animSkipTime > 0.0f)
                SkipSma(prize->sma, prize->animSkipTime);
        } else {
            SetSmaGoal(prize->sma, anim);
        }
    } else if (prize->sma) {
        RetractSma(prize->sma);
        prize->sma->Release();
        prize->sma = 0;
    }
}
} // namespace M_SetDprizeDprizes

namespace M_func_001C6308 {
union U { qword q; float f[4]; };
struct XFS { char pad[0xB0]; qword scale; };   /* the prize's second XF (SO unknown5C) as the glint reads it */
struct Q { char pad[0x18]; float duration; void *owner; char pad20[0xEC-0x20]; void *vptr; void *emit; };
extern qword D_002C0D80 __asm__("D_002C0D80");
extern float D_002C0D90 __asm__("D_002C0D90");
qword ConvertVecBetweenLos(DPRIZE *,void *,qword) __asm__("ConvertVecBetweenLos");
float RandomFloatInRange__Fff(float,float) __asm__("RandomFloatInRange__Fff");
struct Q *NewRipInGroup(SW *,void *,void *) __asm__("NewRipInGroup");
qword GetLoWorldPos(void *) __asm__("GetLoWorldPos");
// Dot product.
extern "C" { static inline float Dot(qword a,qword b)
{
    qword d,one;
    float f;
    __asm__("vmul.xyz %0,%2,%3\n\tvaddw.x %1,$vf0,$vf0w":"=j"(d),"=j"(one):"j"(a),"j"(b));
    __asm__("vadday.x $ACC,%0,%0y\n\tvmaddz.x %0,%1,%0z":"+j"(d):"j"(one));
    __asm__("mtc1 %1,%0":"=f"(f):"r"(d));
    return f;
} }
// Larger of two floats.
extern "C" { static inline float Max(float a,float b)
{
    __asm__("max.s %0,%0,%1":"+f"(a):"f"(b));
    return a;
} }
// spawn sparkle rip (NewRipInGroup) at prize position periodically. (inferred)
// Also known as: UpdatePickupGlint (upstream readable / Sly 1 name; UpdatePickupGlint__FP2LO).
extern "C" void EmitDprizeGlint(DPRIZE *prize)
{
    union U scale;
    qword direction;
    int side;
    void *emitter;
    struct Q *q;
    if(!prize->glintOwner||prize->glintChance<=0)return;
    direction=ConvertVecBetweenLos(prize,0,D_002C0D80);
    side=Dot(direction,g_cm.matq.rot[1])<0;
    if(side!=prize->glintSide) {
        if(RandomFloatInRange__Fff(0,1)<prize->glintChance) {
            if(D_002C0D90<g_clock.time-prize->glintTime) {
                if(Dot(direction,g_cm.matq.rot[0])<0)emitter=prize->leftEmitter;
                else emitter=prize->rightEmitter;
                if(!emitter)goto changed;
                q=NewRipInGroup(&g_sw,(void *)4,0);
                if(!q)goto changed;
                prize->glintTime=g_clock.time;
                {
                    ((RIP *)q)->Init(GetLoWorldPos(emitter), 1.0f, 0);
                }
                {
                    void *owner=prize->glintOwner;
                    q->emit=emitter;
                    q->owner=owner;
                }
                scale.q=((struct XFS *)prize->rotationXf)->scale;
                q->duration=1.5f/Max(1,__builtin_fabsf(scale.f[2]));
            }
        }
changed:
        prize->glintSide=side;
    }
}
} // namespace M_func_001C6308

namespace M_func_001C64F8 {
// Splice setter: the oids of the glint owner and the left / right glint emitters (resolved at post-load).
extern "C" void SetDprizeParams(DPRIZE *p, OID glintOwnerOid, OID leftEmitterOid, OID rightEmitterOid) { p->glintOwnerOid = glintOwnerOid; p->leftEmitterOid = leftEmitterOid; p->rightEmitterOid = rightEmitterOid; }
} // namespace M_func_001C64F8

namespace M_func_001C6508 {
// CID 91 Init (sprayable pickup; coin?). (inferred)
extern "C" void InitCoin(COIN *coin)
{
    InitDprize(coin);
    coin->bounceRadius = 45.0f;
    coin->collectRadius = 65.0f;
    coin->bounceBoostThreshold = 250.0f;
    coin->bounceBoost = 150.0f;
    coin->normalResponse = 0.5f;
    coin->tangentResponse = 0.25f;
    coin->glintChance = 0.25f;
    coin->lifetimeMin = 5.0f;
    coin->lifetimeMax = 5.0f;
}
} // namespace M_func_001C6508

namespace M_func_001C6588 {
void UpdateDprize(DPRIZE *p) __asm__("UpdateDprize");
// COIN Update: UpdateDprize, then while idle counts the lifetime down and starts vanishing at zero.
} // namespace M_func_001C6588
// COIN::Update: prize update, then while idle count the lifetime down and vanish at zero.
// Also known as: UpdateCoin (upstream readable / Sly 1 name; UpdateCoin__FP4COIN).
void COIN::Update() { using namespace M_func_001C6588;
    UpdateDprize(this);
    if (!state) {
        lifetimeLeft -= g_clock.deltaTime;
        if (lifetimeLeft <= 0.0f) SetState(5);
    }
}

namespace M_func_001C65F8 {
struct MAT { qword a[3]; };
struct LISTENER { char pad[0xC]; void *vptr; };
struct X {
    char pad[0x1C]; LO *attachedLo; char p1[0x40 - 0x20]; struct MAT mat;
    char p2[0xEC - 0x70]; void *vptr; void *smackDef; LO *smackTarget; int smackArg;
};
struct M { int n; DPRIZE *p; int pad[14]; };
extern char D_002C0DA0[] __asm__("D_002C0DA0");
float RandomFloatInRange__Fff(float a, float b) __asm__("RandomFloatInRange__Fff");
struct X *NewRipInGroup(void *pg, int a, int b) __asm__("NewRipInGroup");
qword GetLoWorldPos(COIN *p) __asm__("GetLoWorldPos");
struct MAT *GetLoWorldMatrix(COIN *p) __asm__("GetLoWorldMatrix");
void AwardCoin(COIN *p, int n) __asm__("AwardCoin");
void *CreateSfxSound(int n) __asm__("CreateSfxSound");
void SetSndSourceLo(void *p, int n) __asm__("SetSndSourceLo");
void RestartHudWidget1(void *pg) __asm__("RestartHudWidget1");
void BroadcastGameEvent(void *p, int n, COIN *ps) __asm__("BroadcastGameEvent");
void SetDprizeDprizes(DPRIZE *p, int n) __asm__("SetDprizeDprizes");
// CID 91 state setter: on collect play sound, emit rip, award (AwardCoin). (inferred)
// Also known as: SetCoinState (upstream readable / Sly 1 name; SetCoinState__FP2LOi).
extern "C" void SetCoinDprizes(COIN *coin, int n)
{
    if (coin->state == n) return;
    switch (n) {
    case 0:
        coin->lifetimeLeft = RandomFloatInRange__Fff(coin->lifetimeMin, coin->lifetimeMax);
        break;
    case 1: {
        struct X *px = NewRipInGroup(&g_sw, 9, 0);
        struct LISTENER *plo;
        if (px) {
            px->smackTarget = coin;
            px->smackDef = D_002C0DA0;
            px->smackArg = 0;
            ((RIP *)px)->Init(GetLoWorldPos(coin), 1.0f, 0);
            px->attachedLo = coin;
            px->mat = *GetLoWorldMatrix(coin);
        } else {
            AwardCoin(coin, 0);
        }
        SetSndSourceLo(CreateSfxSound(0x29), 0);
        plo = (struct LISTENER *)coin->collectListener;
        if (plo) {
            struct M m;
            m.n = 1;
            m.p = coin;
            ((EXPL *)plo)->ExplodeExplso((EXPLSO *)&m);   /* M: EXPLSO grf 1 and ploReference */
        }
        n = 2;
        coin->HandleSpliceEvent(5, 0, 0, 0, 0);
        RestartHudWidget1(&g_sw);
        BroadcastGameEvent(&g_game, 0x12, coin);
        break;
    }
    }
    SetDprizeDprizes(coin, n);
}
} // namespace M_func_001C65F8

// increments a game counter via 0x1617E0/0x161798. (inferred)
extern "C" void AwardCoin()
{
    SetCoinCount(&g_game, GetCoinCount(&g_game) + 1);
}

namespace M_func_001C67D8 {
// CID 93 Init (sprayable pickup 0x5D). (inferred)
extern "C" void InitMedicine(MEDICINE *medicine)
{
    InitDprize(medicine);
    medicine->bounceRadius = 45.0f;
    medicine->collectRadius = 75.0f;
    medicine->bounceBoostThreshold = 250.0f;
    medicine->bounceBoost = 50.0f;
    medicine->tangentResponse = 0.2f;
    medicine->normalResponse = 0.65f;
    medicine->glintChance = 0.25f;
    medicine->healArg0 = 10;
    medicine->healArg1 = 20;
    medicine->lifetimeMin = 4.0f;
    medicine->lifetimeMax = 5.0f;
}
} // namespace M_func_001C67D8

namespace M_func_001C6880 {
void UpdateDprize(DPRIZE *p) __asm__("UpdateDprize");
// MEDICINE Update: as COIN's (lifetime countdown while idle, then vanish).
} // namespace M_func_001C6880
// MEDICINE::Update: prize update, then while idle count the lifetime down and vanish at zero.
void MEDICINE::Update() { using namespace M_func_001C6880;
    UpdateDprize(this);
    if (!state) {
        lifetimeLeft -= g_clock.deltaTime;
        if (lifetimeLeft <= 0.0f) SetState(5);
    }
}

namespace M_func_001C68F0 {
struct MAT { qword a[3]; };
struct LISTENER { char pad[0xC]; void *vptr; };
struct X {
    char pad[0x1C]; LO *attachedLo; char p1[0x40 - 0x20]; struct MAT mat;
    char p2[0xEC - 0x70]; void *vptr; void *smackDef; LO *smackTarget; int smackArg;
};
struct M { int n; DPRIZE *p; int pad[14]; };
extern char D_002C0E10[] __asm__("D_002C0E10");
float RandomFloatInRange__Fff(float a, float b) __asm__("RandomFloatInRange__Fff");
struct X *NewRipInGroup(void *pg, int a, int b) __asm__("NewRipInGroup");
PO *GetHeroPo(void *pg) __asm__("GetHeroPo");
qword GetLoWorldPos(MEDICINE *p) __asm__("GetLoWorldPos");
struct MAT *GetLoWorldMatrix(MEDICINE *p) __asm__("GetLoWorldMatrix");
void OnMedicineSmackDone(MEDICINE *p, int n) __asm__("OnMedicineSmackDone");
void *CreateSfxSound(int n) __asm__("CreateSfxSound");
void SetSndSourceLo(void *p, int n) __asm__("SetSndSourceLo");
void RestartHudWidget1(void *pg) __asm__("RestartHudWidget1");
void BroadcastGameEvent(void *p, int n, MEDICINE *ps) __asm__("BroadcastGameEvent");
void SetDprizeDprizes(DPRIZE *p, int n) __asm__("SetDprizeDprizes");
// CID 93 setter (uses GetCurrentPo - gives to current character). (inferred)
// Also known as: SetCharmState (upstream readable / Sly 1 name; SetCharmState__FP2LOi).
extern "C" void SetMedicineDprizes(MEDICINE *medicine, int n)
{
    if (medicine->state == n) return;
    switch (n) {
    case 0:
        medicine->lifetimeLeft = RandomFloatInRange__Fff(medicine->lifetimeMin, medicine->lifetimeMax);
        break;
    case 1: {
        PO *q = GetHeroPo(&g_sw);
        struct X *px;
        struct LISTENER *plo;
        if (!q->CanBeHealed()) {
            n = medicine->state;
            break;
        }
        px = NewRipInGroup(&g_sw, 9, 0);
        if (px) {
            px->smackTarget = medicine;
            px->smackDef = D_002C0E10;
            px->smackArg = 0;
            ((RIP *)px)->Init(GetLoWorldPos(medicine), 1.0f, 0);
            px->attachedLo = medicine;
            px->mat = *GetLoWorldMatrix(medicine);
        } else {
            OnMedicineSmackDone(medicine, 0);
        }
        q->AddHpMp(medicine->healArg0, medicine->healArg1);
        plo = (struct LISTENER *)medicine->collectListener;
        if (plo) {
            struct M m;
            m.n = 1;
            m.p = medicine;
            ((EXPL *)plo)->ExplodeExplso((EXPLSO *)&m);   /* M: EXPLSO grf 1 and ploReference */
        }
        n = 2;
        medicine->HandleSpliceEvent(5, 0, 0, 0, 0);
        break;
    }
    }
    SetDprizeDprizes(medicine, n);
}
} // namespace M_func_001C68F0

// Empty callback: nothing to do when the medicine pickup's smack animation ends.
extern "C" void OnMedicineSmackDone()
{
}

// KEY::Init: prize init with the key's bounce and collection tuning.
void KEY::Init() {
    InitDprize(this);
    bounceRadius = 35.0f;
    collectRadius = 35.0f;
    bounceBoostThreshold = 500.0f;
    bounceBoost = 250.0f;
    tangentResponse = 0.6f;
    normalResponse = 0.6f;
    glintChance = 0.75f;
}

namespace M_func_001C6B28 {
void *CreateSfxSound(int n) __asm__("CreateSfxSound");
void SetSndSourceLo(void *p, int n) __asm__("SetSndSourceLo");
void RequestTaskState(void *p, int n, KEY *plo) __asm__("RequestTaskState");
void SetDprizeDprizes(KEY *p, int n) __asm__("SetDprizeDprizes");
// CID 92 setter (sound on collect). (inferred)
// Also known as: SetPrizeState (upstream readable / Sly 1 name; SetPrizeState__FP2LOi).
extern "C" void SetKeyDprizes(KEY *key, int n)
{
    if (key->state == n) return;
    if (n == 1) {
        SetSndSourceLo(CreateSfxSound(0), 0);
        if (key->task) RequestTaskState(key->task, 2, key);
        n = 2;
        key->HandleSpliceEvent(5, 0, 0, 0, 0);
    }
    SetDprizeDprizes(key, n);
}
} // namespace M_func_001C6B28

namespace M_LaunchSprayedDprize {
extern char D_002A2D00[] __asm__("D_002A2D00");
extern const float D_002A7758 __asm__("D_002A7758");
void SetLoParent(DPRIZE *p, int a, int b) __asm__("SetLoParent");
// Zeroes a 3-float vector.
extern "C" { static inline void Zero(VECTOR4 *pv)
{
    pv->z = 0.0f;
    pv->y = 0.0f;
    pv->x = 0.0f;
} }
// re-parent spawned prize (SetLoParent) and init motion. (inferred)
extern "C" void LaunchSprayedDprize(DPRIZE *prize, qword q1, qword q2, float f1, float f2)
{
    prize->Add();
    SetLoParent(prize, 0, 0);
    prize->TranslateToPos(q1);
    prize->RotateToMat((MATRIX3 *)D_002A2D00);   /* the identity */
    Zero(&prize->positionXf->velocity);
    Zero(&prize->rotationXf->velocity);
    prize->animSkipTime = f1;
    prize->hasBoosted = 0;
    prize->SetState(3);
    prize->velocity = q2;
    prize->expireTime = (f2 == D_002A7758) ? D_002A7758 : g_clock.time + f2;
}
} // namespace M_LaunchSprayedDprize

namespace M_PdprizeNewSpray {
void *FindSwObject__FP2SWi3OIDP2LO(void *p, int a, int id, LO *b) __asm__("FindSwObject__FP2SWi3OIDP2LO");
DPRIZE *AcquireProxyObject(void *pg, int n) __asm__("AcquireProxyObject");
void LockProxyObject(void *pg, DPRIZE *plo) __asm__("LockProxyObject");
// make one prize of kind 0x5B/0x5D from its proxy pool (AcquireProxyObject + LockProxyObject). (inferred)
// Also known as: CpickupInstances (upstream readable / Sly 1 name; CpickupInstances__FPviiPP2LO).
extern "C" int NewSprayedDprize(void *prize, int kind, int n, DPRIZE **aplo)
{
    int id, i, c;
    void *pg;
    DPRIZE *plo;
    if (!n) return 0;
    switch (kind) {
    case 0x5B: id = 0x2B3; break;
    case 0x5D: id = 0x2B4; break;
    default: return 0;
    }
    pg = FindSwObject__FP2SWi3OIDP2LO(prize, 5, id, 0);
    if (!pg) return 0;
    c = 0;
    for (i = 0; ; i++) {
        if (i >= n) break;
        plo = AcquireProxyObject(pg, 0);
        if (!plo) break;
        plo->SetState(2);
        c++;
        LockProxyObject(pg, plo);
        aplo[i] = plo;
    }
    return c;
}
} // namespace M_PdprizeNewSpray

namespace M_func_001C6DE8 {
// FindDprizeLanding: find a landing point on the objects below/above `point` (used three times by SprayDprizes, which sits
// right after it in the same file and shares its FRAME / VU_FLOAT / VV / AddScaled idioms). Casts a line from point up to
// point + axis * gMax and, failing that, down to point + axis * gMin, against `apcand` (fetched with IntersectSwBoundingBox when the
// caller passed none); returns the hit position, or point itself when gMin >= 0 and nothing above was found.
// Matching notes (wave 4, lane 1):
//  * FRAME is a plain RAII local with out-of-line ctor/dtor (as in SprayDprizes); g++ emits the three `dtor(this, 2)` calls
//    and cross-jumping rebuilds the target's shared tails. Parameters are used directly (their homes are 0x80/0x84).
//  * The game loads IntersectSwBoundingBox's third argument (posMax) into $a2 BEFORE the `apcand == 0` test: that load is the last insn
//    of block 0, so reorg puts it in the bnez delay slot, and its write of $a2 gives the `sw a2,0x84` home store one more
//    dependent, which is why that store precedes the $ra/$f20 saves in the prologue. cse merges any plain copy made before the
//    test, so the argument is held in a variable pinned to $a2 (the one allowed pin).
struct LSG {
    QWROW posA;           // 0x00: hit point (lower cast)
    QWROW posB;           // 0x10: hit point (upper cast)
    QWROW normalA;        // 0x20
    QWROW normalB;        // 0x30
    float t;                // 0x40
    float u;                // 0x44
    char pad48[0x18];
};
struct CANDIDATE;
struct VU_FLOAT {
    qword q;
    // Broadcast a float into a VU0 register.
    VU_FLOAT(float f) { __asm__("qmtc2.ni %1,%0" : "=j"(q) : "r"(f)); }
};
// Qword wrapper convertible to and from qword.
struct VV { qword q; VV() {} VV(const qword &v) { q = v; } operator qword() const { return q; } };

int IsDprizeCollisionCandidate(void *, CANDIDATE *) __asm__("IsDprizeCollisionCandidate");
void IntersectSwBoundingBox(void *, qword, qword, int (*)(void *, CANDIDATE *), void *, int *, CANDIDATE ***) __asm__("IntersectSwBoundingBox");
CANDIDATE *PsoHitTestLineObjects(int, qword, qword, int, CANDIDATE **, LSG *)
    __asm__("FindFirstObjectHitByLine");
extern qword g_normalZ __asm__("g_normalZ");

// base + v * g
static inline qword AddScaled(VV base, VU_FLOAT g, qword v)
{
    qword r;
    __asm__("vadda.xyzw $ACC,%1,$vf0\n\tvmaddx.xyzw %0,%2,%3x" : "=j"(r) : "j"(base.q), "j"(v), "j"(g.q));
    return r;
}

// find landing point on objects below/above a point. (inferred)
// Also known as: FFindPickupFloor (upstream readable / Sly 1 name;
//   FFindPickupFloor__FfffG9VU_VECTORiP6VECTORPG9VU_VECTOR).
extern "C" int FindDprizeLanding(float gMin, float gMax, float gThreshold, qword point, int ccand, CANDIDATE **apcand,
                             qword *ppos)
{
    LSG lsg;
    STACKSCOPE frame;
    qword posMax = AddScaled(point, VU_FLOAT(gMax), g_normalZ);
    qword posMin = AddScaled(point, VU_FLOAT(gMin), g_normalZ);
    register qword posMaxArg __asm__("$6") = posMax;   // pin: the game loads this $a2 argument before the test (see top)
    if (!apcand)
        IntersectSwBoundingBox(&g_sw, point, posMaxArg, IsDprizeCollisionCandidate, 0, &ccand, &apcand);
    if (gMax > 0) {
        if (PsoHitTestLineObjects(0, point, posMax, ccand, apcand, &lsg) && lsg.t == 0) {
            if (lsg.u == 1 || lsg.normalB.f[2] < gThreshold)
                return 0;
            *ppos = lsg.posB.q;
            return 1;
        }
    }
    if (gMin < 0) {
        if (PsoHitTestLineObjects(0, point, posMin, ccand, apcand, &lsg)) {
            if (lsg.t == 0 || lsg.normalA.f[2] < gThreshold)
                return 0;
            *ppos = lsg.posA.q;
            return 1;
        }
        return 0;
    }
    *ppos = point;
    return 1;
}
} // namespace M_func_001C6DE8

namespace M_SprayDprizes {
// SprayDprizes (wave-3 lane 10): spray up to cRequested objects of a kind (0x5B / 0x5D, made by NewSprayedDprize) out of a
// point. The seed is dropped to the ground (posOrig), snapped onto the region (posSnap, height from posOrig or from the
// nearest object) and raised again; then up to 50 candidate landing points are tried on a widening spiral around posSnap.
// A candidate is used when it is blocked from posSnap and its sideways-pushed partner, and a landing spot is found near it
// among the objects in the box around posSnap; the object is launched ballistically from posOrig to land there after a
// time from ComputeMaxBallisticTime (staggered per object). Objects that found no spot are put back into state 2.
// Matching notes (found on the -da dumps):
//  * PvAllocFrame keeps its ALLOCREQ in the inline's own block, so the slot is freed and posMin reuses it (sp+0x30) like
//    the game; posLand is declared in the loop body after the first temporaries so it shares sp+0x50 with them.
//  * The z of posSnap is stored in both arms (cross-jumped later): sched1 then sees the join block without the store, which
//    gives the game's load order, and cse1 no longer carries &posSnap into the else arm (gcse makes the $a2/$s1 copy).
//  * The box corners: posMin first (its VU_FLOAT temporary is stored to sp+0x50 first); AddScaled takes (VV base, scalar,
//    vector), so the scalar is converted before the Normalize branch and the base is a separate pseudo (the vmove).
//  * Scale lists the dot product first (local-alloc ties the projection to it); GClamp passes both bounds by value.
//  * posBase is read before the SetVectorCylind call, so it is a spilled pseudo at sp+0x80 like the game.
union VECTOR4 { qword q; float f[4]; };
// Allocation request: byte count and alignment.
struct ALLOCREQ { int cb, cbAlign; ALLOCREQ(int c, int a) { cb = c; cbAlign = a; } };
struct REGION;
struct CANDIDATE;
extern qword D_002A2DA0 __asm__("D_002A2DA0"), g_normalZ __asm__("g_normalZ"), D_002A2DE0 __asm__("D_002A2DE0"), D_002C0D60 __asm__("D_002C0D60");
extern float D_002A775C __asm__("D_002A775C"), D_002A7760 __asm__("D_002A7760"), D_002C0D70 __asm__("D_002C0D70");
extern float D_002C0E80 __asm__("D_002C0E80"), D_002C0E84 __asm__("D_002C0E84"), D_002C0E88 __asm__("D_002C0E88"), D_002C0E8C __asm__("D_002C0E8C"), D_002C0E90 __asm__("D_002C0E90"), D_002C0E94 __asm__("D_002C0E94");
extern CLQ D_002C0EA0 __asm__("D_002C0EA0");
extern LM D_002C0EB0 __asm__("D_002C0EB0");
extern float D_002C0EB8 __asm__("D_002C0EB8"), D_002C0EBC __asm__("D_002C0EBC"), D_002C0EC0 __asm__("D_002C0EC0"), D_002C0EC4 __asm__("D_002C0EC4");

int FindDprizeLanding(float, float, float, qword, int, CANDIDATE **, qword *) __asm__("FindDprizeLanding");
SO *GetHeroPo(void *) __asm__("GetHeroPo");
REGION *FindPathzoneAtPos(qword, int) __asm__("FindPathzoneAtPos");
int FindPathzoneClosestPoint(REGION *, qword, qword *, int) __asm__("FindPathzoneClosestPoint");
int IsEdgeBlockedByPathzone(REGION *, qword, qword, int) __asm__("IsEdgeBlockedByPathzone");
qword GetLoWorldPos(SO *) __asm__("GetLoWorldPos");
void *AllocFromStack(ALLOCREQ *) __asm__("AllocFromStack");
int NewSprayedDprize(void *, int, int, DPRIZE **) __asm__("NewSprayedDprize");
int IsDprizeCollisionCandidate(void *, CANDIDATE *) __asm__("IsDprizeCollisionCandidate");
void IntersectSwBoundingBox(void *, qword, qword, int (*)(void *, CANDIDATE *), void *, int *, CANDIDATE ***) __asm__("IntersectSwBoundingBox");
float RandomFloatInRange__Fff(float, float) __asm__("RandomFloatInRange__Fff");
float NormalizeAngle__Ff(float) __asm__("NormalizeAngle__Ff");
qword SetVectorCylind(float, float, float) __asm__("SetVectorCylind");
float ComputeMaxBallisticTime(qword, qword, float, float, float, float) __asm__("ComputeMaxBallisticTime");
void LaunchSprayedDprize(DPRIZE *, qword, qword, float, float) __asm__("LaunchSprayedDprize");

struct VU_FLOAT {
    qword q;
    // Broadcast a float into a VU0 register.
    VU_FLOAT(float f) { __asm__("qmtc2.ni %1,%0" : "=j"(q) : "r"(f)); }
};
// Helpers PvAllocFrame, GetZ, GFromVf, Add, Sub (small asm wrappers/accessors shaped for the match).
static inline void *PvAllocFrame(int cb, int cbAlign) { ALLOCREQ req(cb, cbAlign); return AllocFromStack(&req); }
// Dot product.
static inline qword Dot(qword a, qword b)
{
    qword d, one;
    __asm__("vmul.xyz %0,%2,%3\n\tvaddw.x %1,$vf0,$vf0w\n\tvadday.x $ACC,%0,%0y\n\tvmaddz.x %0,%1,%0z" : "=j"(d), "=&j"(one) : "j"(a), "j"(b));
    return d;
}
// Length of a vector as a float.
static inline qword Length(qword v)
{
    qword d, one, l;
    __asm__("vmul.xyz %0,%2,%2\n\tvaddw.x %1,$vf0,$vf0w\n\tvadday.x $ACC,%0,%0y\n\tvmaddz.x %0,%1,%0z" : "=j"(d), "=&j"(one) : "j"(v));
    __asm__("vsqrt $Q,%1x\n\tvwaitq\n\tvaddq.x %0,$vf0,$Q" : "=j"(l) : "j"(d));
    return l;
}
// Length of the xy part of a vector.
static inline qword LengthXY(qword v)
{
    qword d, l;
    __asm__("vmul.xy %0,%1,%1\n\tvaddy.x %0,%0,%0y" : "=j"(d) : "j"(v));
    __asm__("vsqrt $Q,%1x\n\tvwaitq\n\tvaddq.x %0,$vf0,$Q" : "=j"(l) : "j"(d));
    return l;
}
// v divided by lane x of l.
// Unit vector of v.
static inline qword Normalize(qword v)
{
    qword fallback = D_002A2DA0;
    qword l = Length(v);
    if (QwordX(l) < 0.0001f)
        l = fallback;
    else
        l = VuDivByX(v, l);
    return l;
}
// v scaled by lane x of g.
static inline qword Scale(qword g, qword v) { qword r; __asm__("vmulx.xyzw %0,%2,%1x" : "=j"(r) : "j"(g), "j"(v)); return r; }
// base + v * g
struct VV { qword q; VV() {} VV(const qword &v) { q = v; } operator qword() const { return q; } };
// base + vec * scale.
static inline qword AddScaled(VV base, VU_FLOAT g, qword v)
{
    qword r;
    __asm__("vadda.xyzw $ACC,%1,$vf0\n\tvmaddx.xyzw %0,%2,%3x" : "=j"(r) : "j"(base.q), "j"(v), "j"(g.q));
    return r;
}
// a * ga + b * gb
static inline qword Combine(qword a, VU_FLOAT ga, qword b, VU_FLOAT gb)
{
    qword r;
    __asm__("vmulax.xyzw $ACC,%1,%2x\n\tvmaddx.xyzw %0,%3,%4x" : "=j"(r) : "j"(a), "j"(ga.q), "j"(b), "j"(gb.q));
    return r;
}
// Helpers GFromClq, GClampR, GClamp (small asm wrappers/accessors shaped for the match).
static inline float GFromClq(const CLQ &clq, float g) { return clq.g0 + g * (clq.g1 + g * clq.g2); }
// Clamp g to [minValue, maxValue].
static inline float GClamp(float g, const LM &lm) { return Clamp(g, lm.minValue, lm.maxValue); }
// The region (path zone) an object stands in, through its physics record's REGENT (or null).
static inline REGION *PregionFromLo(SO *plo)
{
    SOPHYS *physics = plo->physics;
    REGENT *pregent = physics ? physics->pregent : 0;
    return pregent ? (REGION *)pregent->pathzone : 0;
}
// Is g beyond gMax?
static inline int FFartherThan(float g, float gMax) { return gMax < g; }

// spray up to cRequested prizes of a kind out of a point onto the ground region. (inferred)
// Also known as: SpawnPickups (upstream readable / Sly 1 name; SpawnPickups__FP6VECTORiG9VU_VECTORPvi).
extern "C" void SprayDprizes(void *pworld, int kind, qword posSeed, void *pvUnused, int cRequested)
{
    VECTOR4 posOrig;
    if (!FindDprizeLanding(0.0f, 300.0f, D_002A775C, posSeed, 0, 0, &posOrig.q))
        return;
    SO *plo = GetHeroPo(pworld);
    REGION *pregion = 0;
    if (plo)
        pregion = PregionFromLo(plo);
    if (!pregion)
        pregion = FindPathzoneAtPos(posOrig.q, 1);
    VECTOR4 posSnap;
    if (FindPathzoneClosestPoint(pregion, posOrig.q, &posSnap.q, 1))
        posSnap.f[2] = posOrig.f[2];
    else {
        qword pos = GetLoWorldPos(plo);
        if (!FindPathzoneClosestPoint(pregion, pos, &posSnap.q, 1))
            return;
        posSnap.f[2] = QwordZ(pos);
    }
    if (FFartherThan(QwordX(LengthXY(VuSub(posOrig.q, posSnap.q))), D_002C0E80))
        return;
    if (!FindDprizeLanding(-250.0f, 1500.0f, D_002A775C, posSnap.q, 0, 0, &posSnap.q))
        return;
    STACKSCOPE frame;
    DPRIZE **aplo = (DPRIZE **)PvAllocFrame(cRequested * sizeof(DPRIZE *), 0x10);
    int clo = NewSprayedDprize(pworld, kind, cRequested, aplo);
    if (!clo)
        return;
    float r = D_002C0E84;
    VECTOR4 posMin, posMax;
    posMin.q = AddScaled(posSnap.q, VU_FLOAT(-r), D_002A2DE0);
    posMax.q = AddScaled(posSnap.q, VU_FLOAT(r), D_002A2DE0);
    int ccand;
    CANDIDATE **apcand;
    IntersectSwBoundingBox(&g_sw, posMin.q, posMax.q, IsDprizeCollisionCandidate, 0, &ccand, &apcand);
    int cLaunched = 0;
    float gAngle = RandomFloatInRange__Fff(-3.1415927f, 3.1415927f);
    float gRadius = D_002C0E8C;
    for (int iAttempt = 0; cLaunched < clo;) {
        float g = GClamp(GFromClq(D_002C0EA0, iAttempt * 0.02f), D_002C0EB0);
        qword posBase = posSnap.q;
        qword posTrial = VuAdd(posBase, SetVectorCylind(gAngle, gRadius, 0.0f));
        qword dpos = VuSub(posTrial, posOrig.q);
        qword dposPerp = VuSub(dpos, Scale(Dot(dpos, g_normalZ), g_normalZ));
        qword posAdjust = AddScaled(posTrial, VU_FLOAT(g), Normalize(dposPerp));
        int fBlocked = !IsEdgeBlockedByPathzone(pregion, posSnap.q, posTrial, 0) && !IsEdgeBlockedByPathzone(pregion, posTrial, posAdjust, 0);
        VECTOR4 posLand;
        if (fBlocked && FindDprizeLanding(-300.0f, 300.0f, 0.7f, posTrial, ccand, apcand, &posLand.q)) {
            qword posTarget = AddScaled(posLand.q, VU_FLOAT(aplo[cLaunched]->bounceRadius - 1.0f), g_normalZ);
            float t = ComputeMaxBallisticTime(VuSub(posTarget, posOrig.q), D_002C0D60, D_002C0EB8, D_002C0EBC, D_002C0EC0, D_002C0EC0);
            t += cLaunched * D_002C0EC4;
            qword v = Combine(VuSub(posTarget, posOrig.q), VU_FLOAT(1.0f / t), D_002C0D60, VU_FLOAT(-0.5f * t));
            DPRIZE *ploLaunch = aplo[cLaunched++];
            LaunchSprayedDprize(ploLaunch, posOrig.q, v, cLaunched * D_002C0D70, D_002A7760);
        }
        gAngle = NormalizeAngle__Ff(gAngle + D_002C0E94);
        gRadius += D_002C0E90;
        if (gRadius > D_002C0E84)
            gRadius -= D_002C0E84 - D_002C0E88;
        if (++iAttempt >= 50)
            break;
    }
    for (; cLaunched < clo; cLaunched++)
        aplo[cLaunched]->SetState(2);
}
} // namespace M_SprayDprizes

namespace M_InitLoot {
// CID 94 Init. (inferred)
extern "C" void InitLoot(LOOT *loot)
{
    InitDprize(loot);
    loot->bounceRadius = 35.0f;
    loot->collectRadius = 40.0f;
    loot->bounceBoostThreshold = 250.0f;
    loot->bounceBoost = 50.0f;
    loot->tangentResponse = 0.6f;
    loot->normalResponse = 0.75f;
    loot->lifetimeMin = 8.0f;
    loot->lifetimeMax = 10.0f;
}
} // namespace M_InitLoot

namespace M_LoadLootFromBrx {
struct STACKSCOPE_STORAGE { int a, b, c, d; };
struct ALLOCREQ_VIEW { int cb; int cbAlign; int pad[2]; };
struct STACKSCOPE_STORAGE *STACKSCOPE_ctor(struct STACKSCOPE_STORAGE *ph) __asm__("STACKSCOPE_ctor");
LOOTE *AllocFromStack(struct ALLOCREQ_VIEW *pal) __asm__("AllocFromStack");
LOOTE *AllocCopyFromWorldHeap(struct ALLOCREQ_VIEW *pal, void *pv) __asm__("AllocCopyFromWorldHeap");
void STACKSCOPE_ctor_1AE4A0(struct STACKSCOPE_STORAGE *ph, int n) __asm__("_$_10STACKSCOPE");
void LoadDprizeFromBrx(LOOT *p, void *pv) __asm__("LoadDprizeFromBrx");
// CID 94 LoadFromBrx (base + copies table on stack heap). (inferred)
extern "C" void LoadLootFromBrx(LOOT *loot, void *pv)
{
    struct STACKSCOPE_STORAGE h;
    struct ALLOCREQ_VIEW al;
    struct ALLOCREQ_VIEW *pal;
    STACKSCOPE_ctor(&h);
    pal = &al;
    pal->cb = 0x580;
    pal->cbAlign = 0x10;
    loot->entries = AllocFromStack(pal);
    LoadDprizeFromBrx(loot, pv);
    ClearCurrentLootEntry(loot);
    pal->cb = loot->entryCount * 0x2C;
    pal->cbAlign = 0x10;
    loot->entries = AllocCopyFromWorldHeap(pal, loot->entries);
    STACKSCOPE_ctor_1AE4A0(&h, 2);
}
} // namespace M_LoadLootFromBrx

namespace M_func_001C7778 {
void UpdateDprize(DPRIZE *p) __asm__("UpdateDprize");
// LOOT Update: as COIN's (lifetime countdown while idle, then vanish).
} // namespace M_func_001C7778
// LOOT::Update: prize update, then while idle count the lifetime down and vanish at zero.
void LOOT::Update() { using namespace M_func_001C7778;
    UpdateDprize(this);
    if (!state) {
        lifetimeLeft -= g_clock.deltaTime;
        if (lifetimeLeft <= 0.0f) SetState(5);
    }
}

namespace M_SetLootDprizes {

void Decref__4CRef(void *p) __asm__("Decref__4CRef");
void func_00129658(void *p, const void *q) __asm__("__as__4CRefRC4CRef");
float RandomFloatInRange__Fff(float a, float b) __asm__("RandomFloatInRange__Fff");
void sprintf(char *buf, const char *fmt, ...) __asm__("sprintf");
void SetDprizeDprizes(void *p, int n) __asm__("SetDprizeDprizes");

struct LISTENER { char pad[0xC]; void *vptr; };
struct M { int n; DPRIZE *p; int pad[14]; };
extern char D_002A7768[] __asm__("D_002A7768");
extern char D_002A7770[] __asm__("D_002A7770");
extern char D_002A7778[] __asm__("D_002A7778");
CRef GetSaveVarByName(char *a, char *b, char *c, int n) __asm__("GetSaveVarByName");
CRef SetSaveVarByName(const CRef &r, char *a, char *b, char *c, int n) __asm__("SetSaveVarByName");
extern void SetLootDprizes(LOOT *p, int n) __asm__("SetLootDprizes");
// CID 94 setter: on collect format id and set save-game key 'mplootkc' (CRef tag 0x15). (inferred)
// Also known as: SetLootState (upstream readable / Sly 1 name; SetLootState__FP2LOi).
void SetLootDprizes(LOOT *loot, int n)
{
    if (loot->state == n) return;
    switch (n) {
    case 0:
        loot->lifetimeLeft = RandomFloatInRange__Fff(loot->lifetimeMin, loot->lifetimeMax);
        break;
    case 1: {
        CRef ref;
        char buf[0x10];
        sprintf(buf, D_002A7768, loot->currentEntry->saveId);
        ref = GetSaveVarByName(D_002A7770, D_002A7778, buf, 0);
        SetSaveVarByName(CRef(ref.u.n + 1, 0x15), D_002A7770, D_002A7778, buf, 0);
        LISTENER *plo = (LISTENER *)loot->collectListener;
        if (plo) {
            M m;
            m.n = n;
            m.p = loot;
            ((EXPL *)plo)->ExplodeExplso((EXPLSO *)&m);   /* M: EXPLSO grf 1 and ploReference */
        }
        n = 2;
        loot->HandleSpliceEvent(5, 0, 0, 0, 0);
        break;
    }
    }
    SetDprizeDprizes(loot, n);
}
} // namespace M_SetLootDprizes

// calls ClearCurrentLootEntry stub. (inferred)
extern "C" void AddLootEntry(LOOT *loot, int id, int saveId)
{
    int count;
    LOOTE *entry;
    ClearCurrentLootEntry(loot);
    count = loot->entryCount;
    if (count < MAX_LOOT_ENTRIES) {
        loot->entryCount = count + 1;
        entry = &loot->entries[count];
        loot->currentEntry = entry;
        entry->id = id;
        entry->saveId = saveId;
        entry->unknown14 = -1;
        entry->unknown18 = -1;
        entry->globCount = 0;
    }
}

// Forgets the loot's current entry.
extern "C" void ClearCurrentLootEntry(LOOT *loot)
{
    loot->currentEntry = 0;
}

namespace M_AddAlarmSensor__FP5ALARM3OID {
// Appends a glob id to the current loot entry (at most 3; SelectLootEntry shows them).
// Also known as: AddAlarmSensor (upstream readable / Sly 1 name; AddAlarmSensor__FP5ALARM3OID).
extern "C" void AddLootEntryObject(LOOT *ploot, OID oid)
{
    LOOTE *p = ploot->currentEntry;
    if ((unsigned)p->globCount < 3) p->globIds[p->globCount++] = oid;
}
} // namespace M_AddAlarmSensor__FP5ALARM3OID

// Id of the loot's current entry, or -1.
extern "C" int GetCurrentLootEntryId(LOOT *loot)
{
    LOOTE *entry = loot->currentEntry;
    int id = -1;
    if (entry != 0) {
        id = entry->id;
    }
    return id;
}

namespace M_func_001C7A58 {
void HideSoAllGlobs(LOOT *p) __asm__("HideSoAllGlobs");
void ShowSoGlobsById(LOOT *p, OID oid) __asm__("ShowSoGlobsById");
// Makes the entry with this id current (the first entry when none matches), then hides the loot's globs
// (HideSoAllGlobs, inferred) and shows the entry's ones. Used by StageLootPouchLoot.
// Also known as: SetPickupLootKind (upstream readable / Sly 1 name; SetPickupLootKind__FP2LOi).
extern "C" void SelectLootEntry(LOOT *loot, int id)
{
    int i, j, count;
    LOOTE *entry;
    count = loot->entryCount;
    /* the exit test inside the loop body is what the original's loop shape needs (MATCHING_NOTES "Loop exit handling") */
    for (j = 0; ; j++) {
        if (j >= count) {
            loot->currentEntry = loot->entries;
            break;
        }
        entry = &loot->entries[j];
        if (id == entry->id) {
            loot->currentEntry = entry;
            break;
        }
    }
    HideSoAllGlobs(loot);
    for (i = 0; i < loot->currentEntry->globCount; i++) ShowSoGlobsById(loot, loot->currentEntry->globIds[i]);
}
} // namespace M_func_001C7A58

// The loot's current entry (or null).
extern "C" LOOTE *GetCurrentLootEntry(LOOT *loot)
{
    return loot->currentEntry;
}

namespace M_FClueCollected {
// Append the clue property and identifier to the selected key array.
struct K { short a, b; };
struct A { int c; short *pk; };
extern const char D_002A7768[] __asm__("D_002A7768"),D_002A7788[] __asm__("D_002A7788");
int GetCurrentLevelIndex__Fv(void) __asm__("GetCurrentLevelIndex__Fv");
int sprintf(char *buf,const char *fmt,...) __asm__("sprintf");
int FindSaveSymbol(SVNHASH *pt,const char *s) __asm__("FindSaveSymbol");
struct A *GetLevelListEntry(int h) __asm__("GetLevelListEntry");
struct A *ConcatSaveKeys(struct A *p, struct A *pa) __asm__("ConcatSaveKeys");
// Index of a save symbol by name.
extern "C" { static inline int Find(SAVEVARS *px, const char *s) { return FindSaveSymbol(&px->symtab,s); } }
// query save data for bit n of 'afClue' for the current level. (inferred)
extern "C" struct A *GetClueSaveKey(int input)
{
    register int n __asm__("$17")=input;
    int h;
    struct A a;
    struct K k;
    char buf[0x10];
    h = GetCurrentLevelIndex__Fv();
    if (h == -1) return 0;
    sprintf(buf, D_002A7768, n);
    k.a = Find(&g_sw.savevars, D_002A7788);
    k.b = n;
    a.c = 2;
    a.pk = (short *)&k;
    return ConcatSaveKeys(GetLevelListEntry(h), &a);
}
} // namespace M_FClueCollected

namespace M_func_001C7BB8 {

void STACKSCOPE_ctor(void *p) __asm__("STACKSCOPE_ctor");
void STACKSCOPE_ctor_1AE4A0(void *p, int n) __asm__("_$_10STACKSCOPE");
void Decref__4CRef(void *p) __asm__("Decref__4CRef");
void *GetClueSaveKey(void *p) __asm__("GetClueSaveKey");

extern void GetGlobalSaveVar(CRefData *pref, void *pv, void *p) __asm__("GetGlobalSaveVar");
// Reads an int global save variable.
static inline int NFromPv(void *pv) { CRefData ref; int n; GetGlobalSaveVar(&ref, pv, &g_gs); n = ref.u.n; Decref__4CRef(&ref); return n; }
extern int IsClueCollected(void *p) __asm__("IsClueCollected");
// wrapper of GetClueSaveKey. (inferred)
int IsClueCollected(void *clue)
{
    STACKSCOPE frame;
    void *pv = GetClueSaveKey(clue);
    if (!pv) return 0;
    return NFromPv(pv);
}
} // namespace M_func_001C7BB8

namespace M_func_001C7C30 {

void *STACKSCOPE_ctor(void *p) __asm__("STACKSCOPE_ctor");
void STACKSCOPE_ctor_1AE4A0(void *p, int n) __asm__("_$_10STACKSCOPE");
void Decref__4CRef(void *p) __asm__("Decref__4CRef");
void __4CRefRC4CRef(void *p, const void *q) __asm__("__4CRefRC4CRef");
void *GetClueSaveKey(int n) __asm__("GetClueSaveKey");
int CountCollectedClues(void) __asm__("CountCollectedClues");
void SetClueCollected(int n, int f) __asm__("SetClueCollected");



extern CRef SetGlobalSaveVar(void *pv, const CRef &ref, void *p) __asm__("SetGlobalSaveVar");
/* Copy the reference into the setter's value argument. */
static inline void SetPv(void *pv, const CRef &r, void *)
{
    CRef arg(r);
    const CRef &ret = SetGlobalSaveVar(pv, arg, &g_gs);
}
// set clue bit / count. (inferred)
void SetClueCollected(int n, int f)
{
    STACKSCOPE frame;
    void *pv = GetClueSaveKey(n);
    if (!pv) return;
    {
        CRef r(f != 0);
        SetPv(pv, r, &g_gs);
    }
    g_sw.numCluesCollected = CountCollectedClues();
}
} // namespace M_func_001C7C30

// Number of clue bottles per level: 30.
extern "C" int GetMaxClueCount()
{
    return 0x1E;
}

namespace M_CClueCollected {
struct K { int a; short b; short c; short d; short e; int f; };
struct S { int k; struct K *pk; };
struct T { SVENT *px; int n; };   /* save.cpp's SVFIND */
extern char D_002A7788[] __asm__("D_002A7788");
int GetCurrentLevelIndex__Fv(void) __asm__("GetCurrentLevelIndex__Fv");
LEVELLIST *GetLevelListEntry(int n) __asm__("GetLevelListEntry");
void CopyAb__FPvT0Ui(struct K *pk, short *a, int b) __asm__("CopyAb__FPvT0Ui");
void *FindSaveVar(void *a, void *b, struct T *pt) __asm__("FindSaveVar");
short FindSaveSymbol(SVNHASH *a, char *s) __asm__("FindSaveSymbol");
unsigned int *GetSaveVarAddress(void *a, void *b, void *c) __asm__("GetSaveVarAddress");
// count collected clue bottles: popcount of afClue words over the level's entries. (inferred)
extern "C" int CountCollectedClues(void)
{
    int n;
    int c;
    struct K k;
    struct S s;
    struct T t;
    unsigned int i;
    SVENT *pe;
    unsigned int v;
    n = GetCurrentLevelIndex__Fv();
    if (n == -1)
        return 0;
    c = 0;
    CopyAb__FPvT0Ui(&k, GetLevelListEntry(n)->a, 6);
    s.pk = &k;
    s.k = 3;
    FindSaveVar(&g_sw.savevars, &s, &t);
    s.k = 5;
    k.d = FindSaveSymbol(&g_sw.savevars.symtab, D_002A7788);
    for (i = t.px->ientChild; i; i = pe->ientSibling) {
        pe = &g_sw.savevars.aent[i];
        k.c = pe->symid;
        v = *GetSaveVarAddress(&g_sw.savevars, &s, &g_gs);
        while (v) { v &= v - 1; c++; }
    }
    return c;
}
} // namespace M_CClueCollected

// CID 95 Init (clue bottle). (inferred)
void CLUE::Init()
{
    InitDprize(this);
    collectRadius = 50.0f;
}

namespace M_func_001C7E68 {
void SO_ResetForProxy(CLUE *p) __asm__("SO_ResetForProxy");
// CID 95 vtable slot 26: SO::VUnk26Proxy base + extra. (inferred)
// Also known as: ResetCluePlacement (upstream readable / Sly 1 name; ResetCluePlacement__FP2LO).
extern "C" void ResetClue(CLUE *clue)
{
    SO_ResetForProxy(clue);
    clue->SetState(0);
}
} // namespace M_func_001C7E68

namespace M_func_001C7EA8 {
void SO_Slot27(CLUE *p) __asm__("SO_Slot27");
// CID 95 vtable slot 27. (inferred)
// Also known as: DetachCluePlacement (upstream readable / Sly 1 name; DetachCluePlacement__FP2LO).
extern "C" void ClearClueState(CLUE *clue)
{
    SO_Slot27(clue);
    clue->SetState(-1);
}
} // namespace M_func_001C7EA8

namespace M_SetClueDprizes {
   /* an EXPL */
struct M { int n; DPRIZE *p; int pad[14]; };
void *CreateSfxSound(int n) __asm__("CreateSfxSound");
void SetSndSourceLo(void *p, int n) __asm__("SetSndSourceLo");
qword GetLoWorldPos(CLUE *p) __asm__("GetLoWorldPos");
void *EmitSwNoise(void *pg, int n, qword q, CLUE *p) __asm__("EmitSwNoise");
CLUEPROXY *GetProxyCopyValue(CLUE *p) __asm__("GetProxyCopyValue");
int IsBasicDerivedFrom__FP5BASIC3CID(LO *p, int n) __asm__("IsBasicDerivedFrom__FP5BASIC3CID");
void SetClueCollected(int n, int f) __asm__("SetClueCollected");
void RestartHudWidget2(void *pg) __asm__("RestartHudWidget2");
void SetDprizeDprizes(CLUE *p, int n) __asm__("SetDprizeDprizes");
// Does the object's GetProxyCopyValue owner derive from class 7?
extern "C" { static inline int FTest(CLUE *p) { CLUEPROXY *q = GetProxyCopyValue(p); return q && IsBasicDerivedFrom__FP5BASIC3CID(q, 7); } }
// CID 95 setter: sound, proxy-copy value, mark clue collected. (inferred)
// Also known as: SetClueState (upstream readable / Sly 1 name; SetClueState__FP2LOi).
extern "C" void SetClueDprizes(CLUE *clue, int n)
{
    CLUEPROXY *q;
    SW *pg;
    qword qv;
    if (n == clue->state) return;
    if (n == 1) {
        struct EXPL *plo = (struct EXPL *)clue->collectListener;
        if (plo) {
            struct M m;
            m.n = n;
            m.p = clue;
            ((EXPL *)plo)->ExplodeExplso((EXPLSO *)&m);   /* M: EXPLSO grf 1 and ploReference */
        } else {
            SetSndSourceLo(CreateSfxSound(0x29), 0);
        }
        pg = &g_sw;
        n = 2;
        qv = GetLoWorldPos(clue);
        EmitSwNoise(pg, 0, qv, clue);
        clue->HandleSpliceEvent(5, 0, 0, 0, 0);
        q = GetProxyCopyValue(clue);
        if (q && IsBasicDerivedFrom__FP5BASIC3CID(q, 7)) {
            CLUEPROXY *plo = GetProxyCopyValue(clue);
            SetClueCollected(plo->clueIndex, 1);
            plo->Remove();
            RestartHudWidget2(pg);
        }
    }
    SetDprizeDprizes(clue, n);
}
} // namespace M_SetClueDprizes

namespace M_func_001C8030 {
void PostLoLoad(LO *p) __asm__("PostLoLoad");
int IsClueCollected(int n) __asm__("IsClueCollected");
// CID 7 (PROXY child) vtable slot 15: base + hide if clue already collected. (inferred)
} // namespace M_func_001C8030
// CLUEPROXY::PostLoad: LO post-load, take the next clue index and hide the proxy when that clue is already collected.
void CLUEPROXY::PostLoad() { using namespace M_func_001C8030; CLUEPROXY *p = this;
    PostLoLoad(p);
    p->clueIndex = g_sw.iclueNext++;
    if (IsClueCollected(p->clueIndex)) p->Remove();
}

namespace RO_ORDER { extern const float r_D_002A7758 __asm__("D_002A7758") = 3.40282347e+38f; }
namespace RO_ORDER { extern const float r_D_002A775C __asm__("D_002A775C") = -3.40282347e+38f; }
namespace RO_ORDER { extern const float r_D_002A7760 __asm__("D_002A7760") = 3.40282347e+38f; }
namespace RO_ORDER { extern const char r_D_002A7768[] __asm__("D_002A7768") = "%d"; }
namespace RO_ORDER { extern const char r_D_002A7770[] __asm__("D_002A7770") = "loot"; }
namespace RO_ORDER { extern const char r_D_002A7778[] __asm__("D_002A7778") = "mplootkc"; }
namespace RO_ORDER { extern const char r_D_002A7788[] __asm__("D_002A7788") = "afClue"; }
namespace M_FVaultOpened {
struct X { int c; short *ps; };
struct B { int a, b; int c; short Find(char *s) ; };



int GetCurrentLevelIndex__Fv(void) __asm__("GetCurrentLevelIndex__Fv");
void *GetLevelListEntry(int n) __asm__("GetLevelListEntry");
int ConcatSaveKeys(void *p, X *px) __asm__("ConcatSaveKeys");
int GetVaultSaveKey(void) __asm__("GetVaultSaveKey");
short FindSaveSymbol(int *p, char *s) __asm__("FindSaveSymbol");

// Index of a save symbol by name.
inline short B::Find(char *s) { return FindSaveSymbol(&c, s); }
// Builds the save key of the current level's 'fVault' flag (null outside a level); like GetClueSaveKey, the name of
// the original (FVaultOpened) suggested a predicate but it returns the key.
int GetVaultSaveKey(void)
{
    int n = GetCurrentLevelIndex__Fv();
    X x;
    short s;
    if (n == -1) return 0;
    s = (*(B *)&g_sw.savevars).Find("fVault");
    x.c = 1;
    x.ps = &s;
    return ConcatSaveKeys(GetLevelListEntry(n), &x);
}
} // namespace M_FVaultOpened

namespace M_func_001C8110 {

void STACKSCOPE_ctor(void *p) __asm__("STACKSCOPE_ctor");
void STACKSCOPE_ctor_1AE4A0(void *p, int n) __asm__("_$_10STACKSCOPE");
void Decref__4CRef(void *p) __asm__("Decref__4CRef");
void *GetVaultSaveKey(void) __asm__("GetVaultSaveKey");

extern void GetGlobalSaveVar(CRefData *pref, void *pv, void *p) __asm__("GetGlobalSaveVar");
// Reads an int global save variable.
static inline int NFromPv(void *pv) { CRefData ref; int n; GetGlobalSaveVar(&ref, pv, &g_gs); n = ref.u.n; Decref__4CRef(&ref); return n; }
extern int IsVaultOpened(void) __asm__("IsVaultOpened");
// Whether the current level's vault has been opened (reads the 'fVault' save flag).
// Also known as: FVaultOpened (upstream readable / Sly 1 name; FVaultOpened__Fv).
int IsVaultOpened(void)
{
    STACKSCOPE frame;
    void *pv = GetVaultSaveKey();
    if (!pv) return 0;
    return NFromPv(pv);
}
} // namespace M_func_001C8110

namespace M_func_001C8188 {

void *STACKSCOPE_ctor(void *p) __asm__("STACKSCOPE_ctor");
void STACKSCOPE_ctor_1AE4A0(void *p, int n) __asm__("_$_10STACKSCOPE");
void Decref__4CRef(void *p) __asm__("Decref__4CRef");
void *__4CRefRC4CRef(void *p, const void *q) __asm__("__4CRefRC4CRef");
void *GetVaultSaveKey(void) __asm__("GetVaultSaveKey");
void AutosaveGame(void *p, int n) __asm__("AutosaveGame");


extern CRef SetGlobalSaveVar(void *pv, const CRef &ref, void *p) __asm__("SetGlobalSaveVar");
extern CRefData *GetGlobalSaveVar(CRefData *pr, void *pv, void *p) __asm__("GetGlobalSaveVar");
// Helpers NFromPv, SetPv (small asm wrappers/accessors shaped for the match).
static inline int NFromPv(void *pv,CRefData &ref) { int n; GetGlobalSaveVar(&ref,pv,&g_gs);n=ref.u.n;Decref__4CRef(&ref);return n; }
// Store r into the global save variable pv.
static inline void SetPv(void *pv,const CRef&r) { CRef arg(r); const CRef &ret=SetGlobalSaveVar(pv,arg,&g_gs); }
extern void SetVaultOpened(int f) __asm__("SetVaultOpened");
// Records in the save whether the current level's vault is open. (inferred)
void SetVaultOpened(int f)
{
    STACKSCOPE frame; CRefData ref;
    void *pv = GetVaultSaveKey();
    if (!pv) return;
    if (f) {
        if (NFromPv(pv,ref) == 0)
            AutosaveGame(&g_save, 0);
    }
    { CRef r(f!=0);SetPv(pv,r); }
}
} // namespace M_func_001C8188

namespace M_InitVault {
void InitPo(VAULT *vault) __asm__("InitPo");
// VAULT vtable slot 7 (Init): PO defaults; the default combination is 123 (0x7B), no dialogs or reward powerup,
// state and dial selection off (-1).
// Also known as: InitVault (upstream readable / Sly 1 name; InitVault__FP5VAULT).
extern "C" void InitVault(VAULT *vault)
{
    sptr v;
    InitPo(vault);
    v = (sptr)vault->ploupd;
    vault->combination = 0x7B;
    vault->openedDialog = -1;
    vault->notEnoughCluesDialog = -1;
    vault->introDialog = -1;
    vault->rewardPowerup = -1;
    vault->state = -1;
    vault->selectedDigit = -1;
    if (IsPointer(v)) ((LOUPD *)v)->grf |= 2; else vault->unknown40 = v | 2;
}
} // namespace M_InitVault

// VAULT vtable slot 15 (PostLoad): binds its parts by oid, finds the dial timer frame, and starts closed (state 0) or
// already opened (state 5) from the save.
void VAULT::PostLoad()
{
    PostSoLoad(this);
    BindLoOidTable(this, 0x18, D_002C0EC8);
    dialTimer = FindSoXf(this, 0x645, 1);
    SetVaultState(this, (IsVaultOpened() == 0) ? 0 : 5);
}

namespace M_func_001C8350 {
void UpdateSo(VAULT *vault) __asm__("UpdateSo");
int IsRelayFiring(void *vault) __asm__("IsRelayFiring");
void UpdateVaultInput(VAULT *vault) __asm__("UpdateVaultInput");
void SetVaultOpened(int f) __asm__("SetVaultOpened");
// VAULT vtable slot 16 (Update). States: 0 closed, 1 intro relay playing, 2 dialling (UpdateVaultInput) until the
// relay ends, then 3 if the entered code equals the combination else 4 (wrong code, back to 0), 3 opening (records
// the vault as opened), 5 open.
// Also known as: UpdateVault (upstream readable / Sly 1 name; UpdateVault__FP2LO).
extern "C" void UpdateVault(VAULT *vault)
{
    int s;
    UpdateSo(vault);
    do {
        s = vault->state;
        switch (s) {
        case 1:
            if (!vault->relay || !IsRelayFiring(vault->relay))
                SetVaultState(vault, 2);
            break;
        case 2:
            if (!vault->relay || !IsRelayFiring(vault->relay)) {
                if (vault->enteredCode == vault->combination)
                    SetVaultState(vault, 3);
                else
                    SetVaultState(vault, 4);
            } else {
                UpdateVaultInput(vault);
            }
            break;
        case 4:
            if (!vault->relay || !IsRelayFiring(vault->relay))
                SetVaultState(vault, 0);
            break;
        case 3:
            if (!vault->relay || !IsRelayFiring(vault->relay)) {
                SetVaultOpened(1);
                SetVaultState(vault, 5);
            }
            break;
        }
    } while (vault->state != s);
}
} // namespace M_func_001C8350

namespace M_func_001C84D8 {
/* (Claude agent) UpdateVaultInput: VU nearest-of-9 selection state machine. The copy of p->index into a register
   variable (a3) reproduces the target's daddu scheduling; register pins as in other VU files. */
union U {qword q;float f[4];};
   /* the dial timer XF: +0xA8 is its end time (so.h names it as an XF pointer) */
struct SGG;struct ALARM;
extern float D_002C0FE8 __asm__("D_002C0FE8"),D_002A7798 __asm__("D_002A7798");
extern qword D_002C0FF0[9] __asm__("D_002C0FF0");
extern int ConsumeButtonsPressedWithin__5POCTLUif(void*,int,float) __asm__("ConsumeButtonsPressedWithin__5POCTLUif");
extern void func_001E0020(void*,ALARM*) __asm__("EnsureSggAlarm__FP3SGGP5ALARM");
extern void SetVaultDialState(VAULT*,int) __asm__("SetVaultDialState");
extern int IsAsegaRetracted(void*) __asm__("IsAsegaRetracted");
// Helpers Pack, Sub, Len, ClockTime, MinimumPage, SelectionTablePage, TableWithMinimum (small asm wrappers/accessors shaped for the match).
static inline qword Pack(float x,float y,float z){register qword result __asm__("$3");register unsigned word __asm__("$2");__asm__("mfc1 %0,%2\n\tmfc1 %1,%3\n\tpextlw %0,%1,%0\n\tmfc1 %1,%4\n\tpcpyld %0,%1,%0":"=&r"(result),"=&r"(word):"f"(x),"f"(y),"f"(z));return result;}
// Length of the xyz part of a as a float (register-pinned).
static inline float Len(qword a){register qword d __asm__("vf1");register qword one __asm__("vf2");float f;__asm__("vmul.xyz %0,%2,%2\n\tvaddw.x %1,$vf0,$vf0w\n\tvadday.x $ACC,%0,%0y\n\tvmaddz.x %0,%1,%0z":"=j"(d),"=&j"(one):"j"(a));__asm__("vsqrt $Q,%0x\n\tvwaitq\n\tvaddq.x %0,$vf0,$Q":"+j"(d));__asm__("mtc1 %1,%0":"=f"(f):"r"(d));return f;}
// The frame clock's current time.
static inline float ClockTime(){float t;__asm__("lwc1 %0,%1":"=f"(t):"m"(g_clock.time));return t;}
// High address page of the minimum constant.
static inline char*MinimumPage(){register char*page __asm__("$6");__asm__("lui %0,%%hi(%1)":"=r"(page):"i"(&D_002A7798));return page;}
// High address page of the selection table.
static inline char*SelectionTablePage(){register char*page __asm__("$5");__asm__("lui %0,%%hi(%1)":"=r"(page):"i"(D_002C0FF0));return page;}
// Address of the selection table and the minimum value (register-pinned).
static inline qword*TableWithMinimum(char*minimumPage,char*tablePage,float*out){register float minimum __asm__("$f1");register qword*table __asm__("$5");__asm__ volatile("lwc1 %0,%%lo(%4)(%2)\n\taddiu %1,%3,%%lo(%5)":"=f"(minimum),"=r"(table):"r"(minimumPage),"r"(tablePage),"i"(&D_002A7798),"i"(D_002C0FF0),"m"(*(const float*)(minimumPage+0x7798)));*out=minimum;return table;}
// Dial input: the stick picks the nearest of nine dial positions (with hysteresis for the current one), a button
// enters it as the next digit (code = code*10 % 1000 + digit + 1), and when the entered code equals the vault's
// combination the vault opens; another button aborts. Runs until the sub-state stops changing.
// Also known as: UpdateVaultDial (upstream readable / Sly 1 name; UpdateVaultDial__FP2LO).
extern "C" void UpdateVaultInput(VAULT*vault){
 union U point;
 int old;
 int divisor=1000;
 do{
  old=vault->dialState;
  if(ConsumeButtonsPressedWithin__5POCTLUif(vault->controller,0x30,0)){
   func_001E0020(vault->relay,(ALARM*)0x655);
   SetVaultDialState(vault,3);
  }
  switch(vault->dialState){
  case 0:
   if(ConsumeButtonsPressedWithin__5POCTLUif(vault->controller,0xC0,0)){SetVaultDialState(vault,2);break;}
   {
    char*minimumPage=MinimumPage();char*tablePage=SelectionTablePage();int current=vault->selectedDigit,best;register int comparison __asm__("$7")=current;
    unsigned i;qword*table;
    float bias,smallest;
    qword pt=Pack(vault->controller->dxLeft,vault->controller->dyLeft,0),vp;bias=D_002C0FE8;__asm__("qmtc2.ni %2,%0":"=j"(vp),"=m"(point.q):"r"(pt));point.q=pt;table=TableWithMinimum(minimumPage,tablePage,&smallest);best=-1;
    for(i=0;i<9;i++){
     float distance=Len(VuSub(vp,table[i]));
     if(i==comparison)distance*=bias;
     if(distance<smallest){smallest=distance;best=i;}
    }
    if(best!=current){vault->selectedDigit=best;SetVaultDialState(vault,1);}
   }
   break;
  case 1:
   if(((struct XF*)vault->dialTimer)->arrivalTime<=ClockTime())SetVaultDialState(vault,0);
   break;
  case 2:
   if(!vault->pasegaDial||IsAsegaRetracted(vault->pasegaDial)){
    int next=(vault->enteredCode*10)%divisor;next++;next+=vault->selectedDigit;
    vault->enteredCode=next;
    if(next==vault->combination){func_001E0020(vault->relay,(ALARM*)0x655);SetVaultDialState(vault,3);}
    else SetVaultDialState(vault,0);
   }
   break;
  }
 }while(vault->dialState!=old);
}

} // namespace M_func_001C84D8

namespace RO_ORDER { extern const float r_D_002A7798 __asm__("D_002A7798") = 3.40282347e+38f; }
namespace M_func_001C8730 {
void StopDialog(void *vault) __asm__("StopDialog");
void StopRelay(void *vault) __asm__("StopRelay");
void SetVaultDialState(VAULT *vault, int n) __asm__("SetVaultDialState");
void ReleaseSwLetterbox(void *vault) __asm__("ReleaseSwLetterbox");
void SetPowerupAcquired(int n, int f) __asm__("SetPowerupAcquired");
int StartDialog(void *vault, int q, int f) __asm__("StartDialog");
void IncrementSwInputLock(void *vault) __asm__("IncrementSwInputLock");
void AcquireSwLetterbox(void *vault) __asm__("AcquireSwLetterbox");
void DecrementSwInputLock(void *vault) __asm__("DecrementSwInputLock");
void FireRelay(void *vault, PO *q) __asm__("FireRelay");
// Vault state change: stops the current dialog and relay, leaves the old state (dialling releases the letterbox; opening
// grants the reward powerup and plays the opened dialog) and enters the new one (dialling takes the letterbox, resets
// the dial and plays the intro dialog; other states fire the vault's own events). (inferred)
// Also known as: SetVaultState (upstream readable / Sly 1 name; SetVaultState__FP2LOi).
extern "C" void SetVaultState(VAULT *vault, int n)
{
    void *pv;
    if (n == vault->state) return;
    if (vault->dialogId) {
        StopDialog(&g_sndmgr);
        vault->dialogId = 0;
    }
    if (vault->relay)
        StopRelay(vault->relay);
    switch (vault->state) {
    case 2:
        SetVaultDialState(vault, -1);
        ReleaseSwLetterbox(&g_sw);
        break;
    case 3:
        if (vault->rewardPowerup != -1)
            SetPowerupAcquired(vault->rewardPowerup, 1);
        if (vault->openedDialog)
            vault->dialogId = StartDialog(&g_sndmgr, vault->openedDialog, 1);
        break;
    case 0:
        if (vault->user)
            IncrementSwInputLock(&g_sw);
        break;
    }
    vault->state = n;
    vault->stateTime = g_clock.time;
    switch (n) {
    case -1:
        return;
    case 2:
        vault->selectedDigit = 5;
        SetVaultDialState(vault, 1);
        AcquireSwLetterbox(&g_sw);
        if (vault->introDialog)
            vault->dialogId = StartDialog(&g_sndmgr, vault->introDialog, 0);
        break;
    case 1:
        vault->SetCtlState(1);
        break;
    case 0:
    case 5: {
        PO *q = vault->user;
        if (q) {
            q->SetCtlState(1);
            vault->user = 0;
            DecrementSwInputLock(&g_sw);
        }
        break;
    }
    }
    pv = vault->relays[vault->state];
    vault->relay = pv;
    if (pv)
        FireRelay(pv, vault->user);
}
} // namespace M_func_001C8730

namespace M_func_001C8918 {

extern qword D_002C1080 __asm__("D_002C1080");
extern qword D_002C1090 __asm__("D_002C1090");
void DropAsegaRef(void *vault) __asm__("DropAsegaRef");
void SetStepplayerGrabTarget(void *vault, void *q) __asm__("SetStepplayerGrabTarget");
qword ConvertVecBetweenLos(void *vault, void *frame, qword q) __asm__("ConvertVecBetweenLos");
qword ConvertXfVec(void *frame, void *vault, qword q) __asm__("ConvertXfVec");
qword GetLoWorldPos(void *vault) __asm__("GetLoWorldPos");
qword TransformXfPos(void *frame, void *vault, qword q) __asm__("TransformXfPos");
void SetXfRole(VAULT *vault, XF *px, int n) __asm__("SetXfRole");
void ReplaceCurrentAseg__FP4ASEGP3ALOffiPP5ASEGA(void *clip, VAULT *owner, float position, float speed, int flags, void *output) __asm__("ReplaceCurrentAseg__FP4ASEGP3ALOffiPP5ASEGA");
// Loads the vault dial's selected index and its timer object (register-pinned for the match).
extern "C" { static inline void Selection(const VAULT*vault,int*index,struct XF**timer){
 register int selected __asm__("$2");register struct XF*receiver __asm__("s0");
 __asm__("lw %0,0x194(%2)\n\tlw %1,0x204(%2)":"=&r"(selected),"=r"(receiver):"r"(vault),"m"(vault->selectedDigit),"m"(vault->dialTimer));
 *index=selected;*timer=receiver;
} }
// Dial sub-state change (-1 off, 0 choosing, 1 turning to the chosen digit, 2 entering it, 3 done): starts the dial
// animation and timer for the new sub-state. (inferred)
// Also known as: SetVaultSub (upstream readable / Sly 1 name; SetVaultSub__FP2LOi).
extern "C" void SetVaultDialState(VAULT *vault, int n)
{
    struct XF *px;
    qword q;
    if (n == vault->dialState) return;
    DropAsegaRef(&vault->pasegaDial);
    vault->dialState = n;
    vault->dialStateTime = g_clock.time;
    switch (n) {
    case -1:
        vault->selectedDigit = -1;
        SetStepplayerGrabTarget(vault->user, 0);
        break;
    case 0:
        SetStepplayerGrabTarget(vault->user, vault->digitTargets[vault->selectedDigit]);
        break;
    case 1:
        SetStepplayerGrabTarget(vault->user, vault->dialCenter);
        ((struct XF *)vault->dialTimer)->arrivalTime = g_clock.time + 0.25f;
        px = (struct XF *)vault->dialTimer;
        q = ConvertXfVec(0, px->parent, ConvertVecBetweenLos(vault->user, 0, D_002C1080));
        px->velq.q = q;
        {int selected;Selection(vault,&selected,&px);
        q=TransformXfPos(0,px->parent,GetLoWorldPos(vault->digitTargets[selected]));}
        px->restValue = q;
        px = (struct XF *)vault->dialTimer;
        q = ConvertXfVec(0, px->parent, ConvertVecBetweenLos(vault->user, 0, D_002C1090));
        px->rate = q;
        SetXfRole(vault, vault->dialTimer, 0x11);
        break;
    case 2:
        SetStepplayerGrabTarget(vault->user, vault->digitTargets[vault->selectedDigit]);
        ReplaceCurrentAseg__FP4ASEGP3ALOffiPP5ASEGA(vault->digitAnims[vault->selectedDigit], vault, 0.0f, 1.0f, 0, &vault->pasegaDial);
        break;
    }
}

#define OFF(t,m) ((int)&((t*)0)->m)

} // namespace M_func_001C8918
#undef OFF

namespace M_func_001C8AF0 {
void RestartHudWidget2(SW *pg) __asm__("RestartHudWidget2");
void StartDialog(SNDMGR *ph, int n, int m) __asm__("StartDialog");
// Splice entry, when the player uses the vault: with enough clue bottles collected (g_sw counters at +0x4764/+0x4768)
// it remembers the user and starts the intro (state 1); otherwise it shows the clue count HUD and plays the
// not-enough-clues dialog. (inferred)
// Also known as: UseVault (upstream readable / Sly 1 name; UseVault__FP2LOi).
extern "C" void TriggerVault(VAULT *vault, PO *n)
{
    SW *pg = &g_sw;
    if (pg->numCluesCollected >= pg->numClues) {
        vault->user = n;
        SetVaultState(vault, 1);
    } else {
        RestartHudWidget2(pg);
        if (vault->notEnoughCluesDialog != -1) StartDialog(&g_sndmgr, vault->notEnoughCluesDialog, 1);
    }
}
} // namespace M_func_001C8AF0

// ---- Data owned by this TU, in address order (docs/guides/tu-merging.md "Data"). Defined after the functions
// so no function sees cluesCollected definition (keeps the code as matched); the namespaced declarations above refer to these.
struct OIDT { int grfsnip; int oid; int ib; };   /* SNIP record (Sly 1 SnipLoObjects) */

// ---- prize's .data in address order (tu-merging.md "Phase 6: one link mode"): one block after the functions (definition order is emission order); the declarations above are what the functions see.
OIDT D_002C0D40[1] = {{0x200, 0x182, SLY2_OFFSETOF(DPRIZE, smaSource)}};
float D_002C0D4C = -5.0f;
float D_002C0D50 = 5.0f;
VECTOR4 D_002C0D60 = {0.0f, 0.0f, -2250.0f, 0.0f};
float D_002C0D70 = 0.1f;
float D_002C0D74 = 300.0f;
float D_002C0D78 = 600.0f;
float D_002C0D7C = 300.0f;
VECTOR4 D_002C0D80 = {1.0f, 1.0f, 0.0f, 0.0f};
float D_002C0D90 = 1.0f;
/* The two smack records (0x70 bytes each, read through D_002C0DA0 / D_002C0E10 as char arrays): 24 floats, then
 * the done callback as cluesCollected pointer-to-member constant {0xFFFF0000, pfn} at +0x60. */
namespace PRIZE_DATA {
struct SMACKI_DEF { float ag[24]; unsigned int pmfDone[2]; int pad[2]; } __attribute__((aligned(16)));
void p_func_001C67A0() __asm__("AwardCoin");
void p_OnMedicineSmackDone() __asm__("OnMedicineSmackDone");
extern SMACKI_DEF r_D_002C0DA0 __asm__("D_002C0DA0"), r_D_002C0E10 __asm__("D_002C0E10");
SMACKI_DEF r_D_002C0DA0 = {{0.83f, 0.75f, 100.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 2.0f, 0.0f, 0.0f, 1.0f, 3.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 1.0f, 0.0f, 1.0f, -0.9f, 0.0f, 0.0f}, {0xFFFF0000, (unsigned int)p_func_001C67A0}, {0, 0}};
SMACKI_DEF r_D_002C0E10 = {{-0.7f, 0.72f, 100.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 2.0f, 0.0f, 0.0f, 1.0f, 3.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 1.0f, 0.0f, 1.0f, -0.8f, 0.0f, 0.0f}, {0xFFFF0000, (unsigned int)p_OnMedicineSmackDone}, {0, 0}};
}
float D_002C0E80 = 2000.0f;
float D_002C0E84 = 300.0f;
float D_002C0E88 = 25.0f;
float D_002C0E8C = 100.0f;
float D_002C0E90 = 23.0f;
float D_002C0E94 = 1.8f;
CLQ D_002C0EA0 __attribute__((aligned(16))) = {{{200.0f, -250.0f, 0.0f, 0.0f}}};
LM D_002C0EB0 = {0.0f, 150.0f};
float D_002C0EB8 = 850.0f;
float D_002C0EBC = -850.0f;
float D_002C0EC0 = 1.0f;
float D_002C0EC4 = 0.075f;
OIDT D_002C0EC8[0x18] = {{0x200, 0x63C, SLY2_OFFSETOF(VAULT, digitTargets[0])}, {0x200, 0x63D, SLY2_OFFSETOF(VAULT, digitTargets[1])}, {0x200, 0x63E, SLY2_OFFSETOF(VAULT, digitTargets[2])}, {0x200, 0x63F, SLY2_OFFSETOF(VAULT, digitTargets[3])},
    {0x200, 0x640, SLY2_OFFSETOF(VAULT, digitTargets[4])}, {0x200, 0x641, SLY2_OFFSETOF(VAULT, digitTargets[5])}, {0x200, 0x642, SLY2_OFFSETOF(VAULT, digitTargets[6])}, {0x200, 0x643, SLY2_OFFSETOF(VAULT, digitTargets[7])},
    {0x200, 0x644, SLY2_OFFSETOF(VAULT, digitTargets[8])}, {0x200, 0x646, SLY2_OFFSETOF(VAULT, dialCenter)}, {0x200, 0x647, SLY2_OFFSETOF(VAULT, digitAnims[0])}, {0x200, 0x648, SLY2_OFFSETOF(VAULT, digitAnims[1])},
    {0x200, 0x649, SLY2_OFFSETOF(VAULT, digitAnims[2])}, {0x200, 0x64A, SLY2_OFFSETOF(VAULT, digitAnims[3])}, {0x200, 0x64B, SLY2_OFFSETOF(VAULT, digitAnims[4])}, {0x200, 0x64C, SLY2_OFFSETOF(VAULT, digitAnims[5])},
    {0x200, 0x64D, SLY2_OFFSETOF(VAULT, digitAnims[6])}, {0x200, 0x64E, SLY2_OFFSETOF(VAULT, digitAnims[7])}, {0x200, 0x64F, SLY2_OFFSETOF(VAULT, digitAnims[8])}, {0x0, 0x650, SLY2_OFFSETOF(VAULT, relays[1])}, {0x0, 0x651, SLY2_OFFSETOF(VAULT, relays[2])},
    {0x0, 0x652, SLY2_OFFSETOF(VAULT, relays[3])}, {0x0, 0x653, SLY2_OFFSETOF(VAULT, relays[4])}, {0x0, 0x654, SLY2_OFFSETOF(VAULT, relays[5])}};
float D_002C0FE8 = 0.7f;
VECTOR4 D_002C0FF0[9] = {{-0.7f, 0.7f, 0.0f, 0.0f}, {0.0f, 0.8f, 0.0f, 0.0f}, {0.7f, 0.7f, 0.0f, 0.0f},
    {-0.8f, 0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f, 0.0f}, {0.8f, 0.0f, 0.0f, 0.0f}, {-0.7f, -0.7f, 0.0f, 0.0f},
    {0.0f, -0.8f, 0.0f, 0.0f}, {0.7f, -0.7f, 0.0f, 0.0f}};
VECTOR4 D_002C1080 = {0.0f, 0.0f, 0.0f, 0.0f};
VECTOR4 D_002C1090 = {100.0f, 0.0f, 0.0f, 0.0f};
