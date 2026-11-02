// func_001C2EF0 (w3lane8, wave 3): launches `count` objects from an owner's source object towards the player.
// The first object is the owner's existing one (+0x158), the others come from its factory (PloAcquireProxy); new
// objects of class 0x5A are put into state 2.  Each launch aims at a random point around the target: a random
// offset (SetVectorCylind from two random angles and -75) is expressed in a basis built (BuildOrthonormalMatrixZ) from the
// horizontal part of the start->target direction and the up axis, and func_001D69F0 computes the launch velocity.
// Class 0x5A objects are launched through LaunchSprayedDprize with an index-scaled delay; others are reset, parented to the
// source and given the velocity through virtual calls.  func_001C2DB8 cleans up the owner on every exit.
// Matching notes:
//  * `game` is a plain local set before the loop: the address is then a spilled constant that reload rematerializes
//    as `la a0` at the call (lui/addiu into a0).  Taking &D_003DD490 inside the loop lets loop.c hoist only the HIGH part.
//  * Scale takes the dot product first (`vmulx %0,%2,%1x`, like GetPipeGoalMat's VU_FLOAT * VU_VECTOR): the asm output
//    is tied to its first dying input, which gives the original vf numbering.
//  * `count`, start, target and offset are plain values; the stack slots at sp+0x60..0x90 are their spill slots, and
//    `dest` is caller-saved around the duration call.
typedef int qword __attribute__((mode(TI)));
struct M3 { qword r[3]; };
struct VTE { short delta, pad; void *fn; };
struct SO { char pad0[0xC]; VTE *vt; char pad10[0x60]; qword position; };
struct OWNER { char pad0[0x150]; SO *source; void *factory; SO *first; };
struct LM { float gMin, gMax; };
struct GAME { int dummy; };
extern "C" {
extern GAME D_003DD490;
extern qword D_002A2DC0, D_002A2DA0, D_002C0D60;    // up axis, fallback X axis, launch acceleration
extern LM D_002C0CF0, D_002C0CF8, D_002C0CE0;       // random ranges: two offset angles, flight time
extern float D_002C0CE8, D_002C0CEC, D_002A76E8;
void *PpoHero(GAME *);
qword func_001A62E8(void *);
float GRandInRange__Fff(float, float);
qword SetVectorCylind(float, float, float);
M3 BuildOrthonormalMatrixZ(qword, qword);
qword func_001D69F0(qword, qword, qword, float);
int FIsBasicDerivedFrom__FP5BASIC3CID(SO *, int);
void LaunchSprayedDprize(SO *, qword, qword, float, float);
SO *PloAcquireProxy(void *, int);
void func_001C2DB8(OWNER *);
}

static inline float X(qword q) { float f; __asm__("mtc1 %1,%0" : "=f"(f) : "r"(q)); return f; }
static inline qword Sub(qword a, qword b) { qword r; __asm__("vsub.xyzw %0,%1,%2" : "=j"(r) : "j"(a), "j"(b)); return r; }
static inline qword Add(qword a, qword b) { qword r; __asm__("vadd.xyzw %0,%1,%2" : "=j"(r) : "j"(a), "j"(b)); return r; }
static inline qword Dot(qword a, qword b)
{
    qword d, one;
    __asm__("vmul.xyz %0,%2,%3\n\tvaddw.x %1,$vf0,$vf0w\n\tvadday.x $ACC,%0,%0y\n\tvmaddz.x %0,%1,%0z" : "=&j"(d), "=&j"(one) : "j"(a), "j"(b));
    return d;
}
static inline qword Scale(qword s, qword v) { qword r; __asm__("vmulx.xyzw %0,%2,%1x" : "=j"(r) : "j"(s), "j"(v)); return r; }
static inline qword Length(qword v)
{
    qword d, one, len;
    __asm__("vmul.xyz %0,%2,%2\n\tvaddw.x %1,$vf0,$vf0w\n\tvadday.x $ACC,%0,%0y\n\tvmaddz.x %0,%1,%0z" : "=j"(d), "=&j"(one) : "j"(v));
    __asm__("vsqrt Q,%1x\n\tvwaitq\n\tvaddq.x %0,$vf0,Q" : "=j"(len) : "j"(d));
    return len;
}
static inline qword Unit(qword v, qword len)
{
    qword r;
    __asm__("vdiv Q,$vf0w,%2x\n\tvwaitq\n\tvmulq.xyzw %0,%1,Q" : "=j"(r) : "j"(v), "j"(len));
    return r;
}
static inline qword Normalize(qword v, qword fallback)
{
    qword len = Length(v);
    if (X(len) < 0.0001f)
        return fallback;
    return Unit(v, len);
}
static inline qword Transform(const M3 &m, qword q)
{
    qword r;
    __asm__("vmulax.xyz $ACC,%1,%4x\n\tvmadday.xyz $ACC,%2,%4y\n\tvmaddz.xyz %0,%3,%4z"
            : "=j"(r) : "j"(m.r[0]), "j"(m.r[1]), "j"(m.r[2]), "j"(q));
    return r;
}
static inline float Rand(const LM &lm) { return GRandInRange__Fff(lm.gMin, lm.gMax); }
static inline void SetState(SO *p, int state) { VTE *v = p->vt; ((void (*)(void *, int))v[76].fn)((char *)p + v[76].delta, state); }
static inline void Reset(SO *p) { VTE *v = p->vt; ((void (*)(void *))v[8].fn)((char *)p + v[8].delta); }
static inline void SetParent(SO *p, SO *parent) { VTE *v = p->vt; ((void (*)(void *, SO *))v[39].fn)((char *)p + v[39].delta, parent); }
static inline void SetVelocity(SO *p, qword velocity) { VTE *v = p->vt; ((void (*)(void *, qword))v[35].fn)((char *)p + v[35].delta, velocity); }

/** @sly2-annot addr=0x1C2EF0
 * Purpose: Launch count loot objects (first held, rest from proxy pool) toward random points around the
 *   player (VecToReachPosInDt); prizes via 0x1C6BD0 with staggered delay
 * Info: proposed name LaunchCid99LootAtPlayer (T2); class CID99; TU tu_1C2D20
 * Notes: names_v2 B proposed PqwVifsBitmapUpload__Fi (0.785)
 * Confidence: low (draft) [T2 0.55] - evidence: static naming pass; sources: r5b_rest
 * See: docs/reference/functions/gameplay-objects.md#tu_1c2d20-0x1c2d20-0x1c3390,
 *   docs/reference/functions/guards-ai.md#pickpocket-0x1f1768-0x1f2108
 * @end-sly2-annot */
extern "C" SO *func_001C2EF0(OWNER *p, int count)
{
    GAME *game = &D_003DD490;
    SO *first = 0;
    for (int i = 0; i < count; i++) {
        SO *node;
        if (i == 0)
            node = p->first;
        else {
            node = PloAcquireProxy(p->factory, 0);
            if (node && FIsBasicDerivedFrom__FP5BASIC3CID(node, 0x5A))
                SetState(node, 2);
        }
        if (!node)
            break;
        if (!first)
            first = node;
        qword start = p->source->position;
        qword target = func_001A62E8(PpoHero(game));
        float pan = Rand(D_002C0CF0);
        qword offset = SetVectorCylind(pan, Rand(D_002C0CF8), -75.0f);
        qword delta = Sub(target, start);
        qword side = Sub(delta, Scale(Dot(delta, D_002A2DC0), D_002A2DC0));
        M3 mat;
        mat = BuildOrthonormalMatrixZ(Normalize(side, D_002A2DA0), D_002A2DC0);
        qword dest = Add(target, Transform(mat, offset));
        qword velocity = func_001D69F0(start, dest, D_002C0D60, Rand(D_002C0CE0));
        if (FIsBasicDerivedFrom__FP5BASIC3CID(node, 0x5A)) {
            float lifetime;
            if (FIsBasicDerivedFrom__FP5BASIC3CID(node, 0x5B) || FIsBasicDerivedFrom__FP5BASIC3CID(node, 0x5D))
                lifetime = (float)i * D_002C0CEC;
            else
                lifetime = D_002A76E8;
            LaunchSprayedDprize(node, p->source->position, velocity, (float)i * D_002C0CE8, lifetime);
        } else {
            Reset(node);
            SetParent(node, p->source);
            SetVelocity(node, velocity);
        }
    }
    func_001C2DB8(p);
    return first;
}
