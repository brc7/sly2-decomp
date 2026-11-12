// dirblend.cpp -- directional clip blending for the player (Sly 1's BLDIR idea): each of eight directional clips
// (forward/back/left/right variants) records its root velocity, and the blend weights are solved from the desired
// movement vector so the animation's root motion matches the requested velocity.
// Entry points: InitDirectionalBlend (setup, called by grab/murray loaders), UpdateDirectionalBlend (per-frame weights,
// from grab and the physics step), UpdateDirectionalAcceleration (steer input -> acceleration and side state).
// VU0 macro-mode asm with register pins: locals are pinned to vf/f registers, keep statement order.
// TU 0x1A0300..0x1A0B18 (5 functions, address order).
// SLY2_ALIAS: __static_initialization_and_destruction_0=StaticInit_dirblend
// SLY2_ALIAS: _GLOBAL_$I$*=GlobalConstructors_dirblend
#include "sly2/slotheap.h"
#include "sly2/vu0.h"
#include "sly2/aseg.h"
#include "sly2/act.h"

// 0x1A09C0 StaticInit_dirblend and GlobalConstructors_dirblend: g++-generated for the six engine slot pools.
SLY2_INSTANTIATE_SLOT_POOLS()

#include "sly2/stepplayer.h"

typedef union U { qword q; float f[4]; int i[4]; } U;
struct ACTOR; struct MULTI;

/* The clips are aseg.h's ASEG (only the duration, +0x74, is read, also as its bits), FindAsegChannel's CHN gives the
   position curve (CHN::pac, sampled through AC::Evaluate). The eight directional axes are stepplayer.h's BLDIR (weight,
   clip, root velocity); the weight is also read as its bits. */

/* The object the player faces (STEPPLAYER::interactTarget); +0x494: its anchor. */
struct TARGETV { char pad[0x494]; LO *anchor; };

extern "C" {
CHN *FindAsegChannel(ASEG *clip, ACTOR *actor, void *keyFlag, int keyMode);
void EnsureAsegBlendDynamic__FP3ALOiiPviPiT5PP6ASEGBL(ACTOR *actor, int entrySize, int entryCount, BLDIR *entries, int count, int *keys, int *counts, MULTI **blendOut);
void RecalculateAsegBlend(void *blend);
void FaceStepToward(STEP *step, LO *target);
float NormalizeAngle__Ff(float);
qword SetVectorCylind(float, float, float);
MATRIX4Q *GetLoWorldMatrix(LO *lo);
}
float D_002BF9F0 = 0.3f;   /* dirblend's .data: input dead zone (the next 12 bytes are 0: alignment) */

// Measures each active directional clip's root velocity (displacement between t=0 and t=duration, divided by the
// duration) into axes[i].direction, then allocates the 8-entry dynamic animation blend for them and returns it in
// *blendOut. Called once when a move that uses directional blending is set up (grab, Murray).
// Also known as: BindRootBlend (upstream readable / Sly 1 name; BindRootBlend__FP9ROOTBLENDP3ALOPP5MULTI).
extern "C" void InitDirectionalBlend(struct ACTOR *actor, BLDIR *axes, MULTI **blendOut)
{
    BLDIR *axis = axes;
    int i;
    U startPos, endPos, durationVec;
    MULTI *blend;
    for (i = 0; i < 8; i++, axis++) {
        if (axis->animation) {
            CHN *segmentRef = FindAsegChannel(axis->animation, actor, (void *)1, 1);
            if (segmentRef) {
                segmentRef->pac->Evaluate(0.0f, 1.0f, &startPos.q, 0);
                segmentRef->pac->Evaluate(axis->animation->duration, 1.0f, &endPos.q, 0);
                {
                    register qword delta __asm__("vf2");
                    register qword scalar __asm__("vf1");
                    register ASEG *axisClip __asm__("$2") = axis->animation;
                    register unsigned int bits __asm__("$2") = *(unsigned int *)&axisClip->duration;
                    __asm__("qmtc2.ni %1, %0" : "=j"(scalar) : "r"(bits));
                    delta = startPos.q;
                    delta = VuSubRef(delta, endPos.q);
                    durationVec.q = scalar;
                    __asm__("vdiv $Q, $vf0w, %1x\n\tvwaitq\n\tvmulq.xyzw %0, %0, $Q" : "+j"(delta) : "j"(durationVec.q));
                    axis->vRoot = delta;
                }
            }
        }
    }
    EnsureAsegBlendDynamic__FP3ALOiiPviPiT5PP6ASEGBL(actor, sizeof(axes[0]), 8, axes, 0, 0, 0, &blend);
    *blendOut = blend;
}

// x lane of q as a float.
static inline float FloatX(qword q){int bits;float f;__asm__("qmfc2.ni %0,%1":"=r"(bits):"j"(q));__asm__("mtc1 %1,%0":"=f"(f):"r"(bits));return f;}

// x lane of q as a float (through $7).
static inline float Float3(qword q){register int bits __asm__("$7");float f;__asm__("qmfc2.ni %0,%1":"=r"(bits):"j"(q));__asm__("mtc1 %1,%0":"=f"(f):"r"(bits));return f;}

// x lane of q as a float (through $7 and $f1).
static inline float Float1(qword q){register int bits __asm__("$7");register float f __asm__("$f1");__asm__("qmfc2.ni %0,%1":"=r"(bits):"j"(q));__asm__("mtc1 %1,%0":"=f"(f):"r"(bits));return f;}

// Per-frame: picks the axis set for the current side (-1 none, 0 negative, 1 positive), weights each axis by how much
// of the player's acceleration lies along its root velocity (normalized so weights sum to at most 1, the remainder
// going to the idle/head axis), rescales the acceleration to the blended root velocity and reblends `request`.
// Also known as: SteerRootBlend (upstream readable / Sly 1 name; SteerRootBlend__FP5ACTORP9ROOTBLENDPv).
extern "C" void UpdateDirectionalBlend(STEPPLAYER *player, BLDIR *axes, void *request)
{
    int indices[5];
    U total, scalar;
    int i, count, clear, accum;
    register float rate __asm__("$f4");
    float sum;
    register qword rateVec __asm__("vf6");
    if (!request)
        return;
    rate = player->activeAnimation->speed;
    if (!axes[6].animation || !axes[7].animation)
        player->blendSide = -1;
    else if (player->blendSide == -1) {
        if (FloatX(VuDot(player->localVelocity, axes[0].vRoot)) < 0.0f)
            player->blendSide = 1;
        else
            player->blendSide = 0;
    }
    {
        register int ratebits __asm__("$7");
        __asm__("mfc1 %0,%1" : "=r"(ratebits) : "f"(rate));
        rateVec = VuFromFloat<qword>(ratebits);
    }
    for (clear = 7; clear >= 0; clear--)
        axes[clear].weight = 0.0f;
    switch (player->blendSide) {
    case -1:
        if (!axes[7].animation) {
            indices[0] = 6;
            indices[1] = 2;
            indices[2] = 4;
        } else {
            indices[0] = 7;
            indices[1] = 3;
            indices[2] = 5;
        }
        indices[3] = 0;
        indices[4] = 1;
        count = 5;
        break;
    case 0:
        indices[0] = 6;
        indices[1] = 2;
        indices[2] = 4;
        indices[3] = 0;
        count = 4;
        break;
    case 1:
        indices[0] = 7;
        indices[1] = 3;
        indices[2] = 5;
        indices[3] = 1;
        count = 4;
        break;
    }
    sum = 0.0f;
    if (rate > 0.0f) {
        for (i = 1; i < count; i++) {
            BLDIR *entry = &axes[indices[i]];
            float denominator = rate * Float1(VuDot(entry->vRoot, entry->vRoot));
            float along = Float3(VuDot(player->localVelocity, entry->vRoot));
            float weight = FpuMax(along / denominator, 0.0f);
            entry->weight = weight;
            sum += weight;
        }
    }
    {
        BLDIR *head = &axes[indices[0]];
        if (sum < 1.0f)
            head->weight = 1.0f - sum;
        else {
            head->weight = 0.0f;
            for (i = 1; i < count; i++)
                axes[indices[i]].weight /= sum;
        }
    }
    total.i[0] = total.i[1] = total.i[2] = 0;
    for (accum = 7; accum >= 0; accum--) {
        qword k = VuFromFloat<qword>(*(int *)&axes->weight);
        total.q = VuAddScaled(total.q, axes->vRoot, k);
        scalar.q = k;
        axes++;
    }
    {
        qword q;
        __asm__("vmulx.xyzw %0,%1,%2x" : "=j"(q) : "j"(total.q), "j"(rateVec));
        {
            register void *argument __asm__("$4") = request;
            scalar.q = rateVec;
            player->localVelocity = q;
            RecalculateAsegBlend(argument);
        }
    }
}

// The player's velocity (virtual slot 33).
static inline qword GetVel(STEPPLAYER*player){return player->GetVelocity();}
// Set the player's velocity (virtual slot 35).
static inline void SetVel(STEPPLAYER*player,qword v){player->SetVelocity(v);}
// Return v scaled by k.x and store side into the player.
static inline qword ScaleAndSetSide(STEPPLAYER*player,qword v,qword k,int side){qword r;__asm__("vmulx.xyzw %0,%2,%3x\n\tsw %4,%1":"=j"(r),"=m"(player->blendSide):"j"(v),"j"(k),"r"(side));return r;}
// Move a 128-bit GPR value into vf3.
static inline qword ToVu(qword bits){register qword r __asm__("vf3");__asm__("qmtc2.ni %1,%0":"=j"(r):"r"(bits));return r;}
// Remove vi's component along the unit normal ni (register-pinned).
static inline qword ProjectOut(qword ni, qword vi)
{
    register qword n __asm__("vf1") = ni, v __asm__("vf3") = vi;
    register qword dot __asm__("vf2");
    dot = VuDot(v, n);
    __asm__("vmulx.xyzw %0,%1,%0x" : "+j"(dot) : "j"(n));
    v = VuSub(v, dot);
    return v;
}
// Remove the sideways component (along the world matrix's row 1) from the player's velocity; saves the old velocity.
static inline void RemoveSide(STEPPLAYER *player, U *result, U *oldVelocity)
{
    MATRIX4Q *basis;
    qword value;
    oldVelocity->q = ToVu(GetVel(player));
    basis = GetLoWorldMatrix(player);
    value = ProjectOut(basis->r[1].q, oldVelocity->q);
    result->q = value;
    SetVel(player, value);
}
// Blend the directional axes for a positive x input into *out (register-pinned).
static inline void BlendPositive(STEPPLAYER *player, BLDIR *axes, float xin, const U *input, U *out)
{
#ifdef SLY2_EE_ASM
    float x = xin;
    register float y __asm__("$f3"), zero __asm__("$f0"), left __asm__("$f2"), right __asm__("$f0");
    register qword a __asm__("vf2"), b __asm__("vf1"), c __asm__("vf4");
    register qword ka __asm__("vf3"), kb __asm__("vf5"), kc __asm__("vf6");
    register int ybits __asm__("$2"), leftbits __asm__("$2");
    __asm__("lwc1 %1,%3\n\tmtc1 $%2,%0" : "=f"(zero), "=f"(y) : "J"(0), "m"(input->f[1]));
    __asm__("mfc1 %0,%3\n\tmin.s %1,%4,%5\n\tlqc2 %2,%6"
            : "=&r"(ybits), "=f"(left), "=j"(c)
            : "f"(y), "f"(zero), "f"(x), "m"(axes[5].vRoot));
    __asm__("qmtc2.ni %4,%0\n\tneg.s %1,%1\n\tmax.s %2,%5,%6\n\tsqc2 %0,%3"
            : "=j"(ka), "+&f"(left), "=f"(right), "=m"(out->q)
            : "r"(ybits), "f"(zero), "f"(x));
    kb = VuFromFloat<qword>(right);
    __asm__("mfc1 %0,%4\n\tlqc2 %1,%5\n\tqmtc2.ni %0,%2\n\tlqc2 %3,%6"
            : "=&r"(leftbits), "=j"(a), "=j"(kc), "=j"(b)
            : "f"(left), "m"(axes[1].vRoot), "m"(axes[3].vRoot));
    out->q = kb;
    __asm__("vmulax.xyzw ACC,%1,%4x\n\tvmaddax.xyzw ACC,%2,%5x\n\tvmaddx.xyzw %0,%3,%6x"
            : "=j"(ka)
            : "j"(a), "j"(b), "j"(c), "0"(ka), "j"(kb), "j"(kc));
    player->localVelocity = ka;
    out->q = kc;
#else
    /* localVelocity = (axes[1] * y + axes[3] * max(0, x)) + axes[5] * -min(0, x); *out = (-min(0, x), 0, 0, 0) */
    float x = xin, y = input->f[1];
    float left = -FpuMin(0.0f, x), right = FpuMax(0.0f, x);
    VuLanes a = VuGet(axes[1].vRoot), b = VuGet(axes[3].vRoot), c = VuGet(axes[5].vRoot);
    player->localVelocity = VuMake<qword>((a.x * y + b.x * right) + c.x * left, (a.y * y + b.y * right) + c.y * left,
                                         (a.z * y + b.z * right) + c.z * left, (a.w * y + b.w * right) + c.w * left);
    out->q = VuWiden<qword>(left);
#endif
}
// Blend the directional axes for a negative x input into *out (register-pinned).
static inline void BlendNegative(STEPPLAYER *player, BLDIR *axes, float xin, const U *input, U *out)
{
#ifdef SLY2_EE_ASM
    float x = xin;
    register float y __asm__("$f2"), zero __asm__("$f0"), left __asm__("$f3"), right __asm__("$f0"),
        negativeY __asm__("$f2");
    register qword a __asm__("vf3"), b __asm__("vf1"), c __asm__("vf4");
    register qword ka __asm__("vf2"), kb __asm__("vf6"), kc __asm__("vf5");
    __asm__("mtc1 $%2,%0\n\tlwc1 %1,%3" : "=f"(zero), "=f"(y) : "J"(0), "m"(input->f[1]));
    __asm__("min.s %0,%2,%3\n\tlqc2 %1,%4" : "=f"(left), "=j"(c) : "f"(zero), "f"(x), "m"(axes[4].vRoot));
    __asm__("max.s %0,%2,%3\n\tneg.s %1,%4" : "=&f"(right), "=f"(negativeY) : "0"(zero), "f"(x), "f"(y));
    __asm__("qmtc2.ni %2,%0\n\tneg.s %1,%1" : "=j"(kb), "+f"(left) : "r"(right));
    __asm__("qmtc2.ni %2,%0\n\tsqc2 %0,%1" : "=j"(ka), "=m"(out->q) : "r"(negativeY));
    __asm__("qmtc2.ni %3,%0\n\tlqc2 %1,%4\n\tlqc2 %2,%5"
            : "=j"(kc), "=j"(a), "=j"(b)
            : "r"(left), "m"(axes[0].vRoot), "m"(axes[2].vRoot));
    out->q = kb;
    __asm__("vmulax.xyzw ACC,%1,%4x\n\tvmaddax.xyzw ACC,%2,%5x\n\tvmaddx.xyzw %0,%3,%6x"
            : "=j"(ka)
            : "j"(a), "j"(b), "j"(c), "0"(ka), "j"(kb), "j"(kc));
    __asm__("sqc2 %2,%0\n\tsqc2 %3,%1" : "=m"(player->localVelocity), "=m"(out->q) : "j"(ka), "j"(kc));
#else
    /* localVelocity = (axes[0] * -y + axes[2] * max(0, x)) + axes[4] * -min(0, x); *out = (-min(0, x), 0, 0, 0) */
    float x = xin, y = -input->f[1];
    float left = -FpuMin(0.0f, x), right = FpuMax(0.0f, x);
    VuLanes a = VuGet(axes[0].vRoot), b = VuGet(axes[2].vRoot), c = VuGet(axes[4].vRoot);
    player->localVelocity = VuMake<qword>((a.x * y + b.x * right) + c.x * left, (a.y * y + b.y * right) + c.y * left,
                                         (a.z * y + b.z * right) + c.z * left, (a.w * y + b.w * right) + c.w * left);
    out->q = VuWiden<qword>(left);
#endif
}

// Turns a steering input (heading and magnitude relative to the player's facing, optionally facing the lock-on
// target) into the player's acceleration: chooses the side from the lateral input past a dead zone (D_002BF9F0),
// removes the sideways velocity when the side flips, and blends the two side axes' root velocities. (inferred)
// Also known as: MoveAroundTarget (upstream readable / Sly 1 name; MoveAroundTarget__FP3SLYP9ROOTBLENDiff).
extern "C" void UpdateDirectionalAcceleration(STEPPLAYER *player, BLDIR *axes, int enabled, float heading,
                                              float magnitude)
{
    U input, scaleOut, oldVelocity;
    register int side __asm__("$3");
    float zero;
    LO *target;
    if (!enabled)
        return;
    target = player->interactTarget;
    if (target) {
        if (player->nUnk10e4 == 3)
            target = ((TARGETV *)target)->anchor;
        FaceStepToward(player, target);
    }
    zero = 0.0f;
    input.q = SetVectorCylind(NormalizeAngle__Ff(heading - player->targetHeading), magnitude, zero);
    if (D_002BF9F0 < input.f[1]) {
        if (player->blendSide == 0) {
            input.f[1] = zero;
            RemoveSide(player, &scaleOut, &oldVelocity);
        }
        side = 1;
    } else if (input.f[1] < -D_002BF9F0) {
        if (player->blendSide == 1) {
            input.f[1] = zero;
            RemoveSide(player, &scaleOut, &oldVelocity);
        }
        side = 0;
    } else {
        side = player->blendSide;
        if (side == -1)
            side = 0;
    }
    {
        register float x __asm__("$f1") = input.f[0];
        if (side == 1) {
            BlendPositive(player, axes, x, &input, &scaleOut);
        } else {
            BlendNegative(player, axes, x, &input, &scaleOut);
        }
    }
    *(int *)&player->vecLocalVelocity.z = 0;
    {
        unsigned int animWord = (unsigned int)player->activeAnimation;
        register qword rate __asm__("vf2"), acceleration __asm__("vf1"), scaled __asm__("vf1");
        __asm__("lqc2 %0,%2\n\tlw %1,%3"
                : "=j"(acceleration), "+r"(animWord)
                : "m"(player->localVelocity), "m"(*(int *)&((ASEGA *)animWord)->speed));
        rate = VuFromFloat<qword>(animWord);
        scaled = ScaleAndSetSide(player, acceleration, rate, side);
        __asm__("sqc2 %2,%0\n\tsqc2 %3,%1" : "=m"(player->localVelocity), "=m"(scaleOut.q) : "j"(scaled), "j"(rate));
    }
}
