// func_00192BD8: set the step of a hero action context (the struct at actor+0x4C0; func_001911A8 calls it).
// Leaves the old step (cleanup per step / per action), then enters the new one: picks the animation clip, the
// script, the flags and the blend for the new step and starts them on the actor.
//
// Matching notes (wave-5 lane 5, fresh rewrite):
// - p ends up in s2 only because two block-local call-crossing pointers take s0/s1 while p is live (old action 4:
//   `ACTOR *actor = p->pactor` before the position call; action 34: `AUX *aux = p->aux` before func_001A62E8).
// - Normalize: return-style inline plus a split vdiv / vmulq (see Unit) keeps the fallback lq/qmtc2 of the target.
// - old step 7: VuFloat's "r,V" alternative (func_001911A8 idiom) fixes the vt/fn and actor/height registers.
// - step -1 reset: the zeros are a chained `param = value = 0.0f` (0.0f in f0) and sched1 puts the stores that kill
//   -1 / 0.0f first, so their statement order is chosen to give the target's store order.

typedef int qword __attribute__((mode(TI)));
typedef unsigned int uint;

struct VTE { short delta, pad; void *fn; };
struct M3 { qword row[3]; };
struct CLIP { char pad0[0x74]; float duration; };
struct JOY { char pad0[0x1C]; float amount; };
struct ANIMATION;
struct PROGRAM;
struct EFFECT;
struct MANAGER;
struct CAMERA;
struct COMPONENT { void *object; };
struct PROGRAM_ITEM { void *owner; CLIP *clip; char pad8[0xA0 - 8]; };
struct LO { char pad0[0xC]; VTE *vt; char pad10[0x40 - 0x10]; uint flags; };
struct ACTOR {
    char pad0[0xC]; VTE *vt; char pad10[0x40 - 0x10]; uint flags; char pad44[0x150 - 0x44]; JOY *pjoy;
    char pad154[0x190 - 0x154]; qword velocity; char pad1A0[0x1B0 - 0x1A0]; char local[0x20C - 0x1B0]; float tStamp;
    char pad210[0x230 - 0x210]; qword position; qword posOffset; char pad250[0x900 - 0x250]; char secondary[0x9BC - 0x900];
    int x9BC; char pad9C0[0x1024 - 0x9C0]; void *focus; char pad1028[0x1030 - 0x1028]; ANIMATION *animation;
    COMPONENT component; int x1038; char pad103C[0x1048 - 0x103C]; float height; char pad104C[0x1060 - 0x104C];
    qword motion; char pad1070[0x10C0 - 0x1070]; void *anchor; char pad10C4[0x130C - 0x10C4]; int x130C;
    char pad1310[0x1318 - 0x1310]; int x1318; char pad131C[0x18D8 - 0x131C]; int x18D8; char pad18DC[0x19A0 - 0x18DC];
    LO *first; LO *second; char pad19A8[0x1AF0 - 0x19A8]; CLIP *specialClip; char pad1AF4[0x1BCC - 0x1AF4]; int x1BCC;
    char pad1BD0[0x1C30 - 0x1BD0]; PROGRAM_ITEM program; char pad1CD0[0x2048 - 0x1CD0]; EFFECT *effect;
};
struct DATA { char pad0[0x10]; float x; char pad14[0xC]; float y; char pad24[0xC]; float z; };
struct SOURCE {
    char pad0[0xC]; VTE *vt; char pad10[0xE0 - 0x10]; ACTOR *owner; char padE4[0x10C - 0xE4]; float parameter;
    char pad110[0x15C - 0x110]; ACTOR *other; char pad160[0x190 - 0x160]; ACTOR *actor; char pad194[0x1BC - 0x194];
    int enabled;
};
struct AUX {
    ACTOR *owner; char pad4[0x10C - 4]; SOURCE *target; char pad110[4]; DATA *data; char pad118[0x12C - 0x118];
    int enabled; char pad130[0x150 - 0x130]; qword position; float value;
};
struct SETTING { void *focus; };
struct ATTACH { char pad0[0x30]; CLIP *clip; char pad34[0x84 - 0x34]; SOURCE *focus; char pad88[0xB0 - 0x88]; };
struct CONTEXT {
    ACTOR *pactor; float tStamp; int action; int step; float tStep; SOURCE *focus; SETTING *setting; float param;
    float value; int reversed; int index; int previous; char pad30[4]; int changed; char pad38[0x68 - 0x38]; AUX *aux;
    char pad6C[0x170 - 0x6C]; CLIP *clip170, *clip174, *clip178, *clip17C; PROGRAM_ITEM program180;
    char pad220[0x230 - 0x220]; PROGRAM *script230; CLIP *clip234, *clip238; char pad23C[4]; PROGRAM_ITEM program240;
    char pad2E0[0x314 - 0x2E0]; CLIP *overrideClip; float gain318, gain31C; CLIP *clip320, *clip324, *clip328;
    PROGRAM *script32C; char pad330[0x430 - 0x330]; CLIP *clip430; char pad434[0x440 - 0x434]; ATTACH attach;
    CLIP *clip4F0, *clip4F4, *clip4F8; int tag; int fInhibit; char pad504[0x50C - 0x504];
    CLIP *clip50C, *clip510, *clip514;
};
struct CLOCK { float t, dt; };
struct VU_FLOAT { qword q; };
union U { qword q; float f[4]; int i[4]; };

extern "C" {
extern CLOCK D_002DDED0;          // g_clock
extern ACTOR *D_002E1E40;         // the player
extern ACTOR *D_002F7900;
extern MANAGER D_002DDF40;
extern qword D_002A2DA0, D_002A2DC0, D_002A2DD0, D_002BF650;
extern float D_002BF63C, D_002BF640;
int FIsBasicDerivedFrom__FP5BASIC3CID(void *, int);
qword func_001A62E8(void *);
M3 *func_001A6318(void *);
void func_001E6F60(SOURCE *, qword, void *, void *, void *, float *, float *);
void func_00183B80(SOURCE *, int);
void SubscribeLoObject(SOURCE *, ACTOR *);
void UnsubscribeLoObject(SOURCE *, ACTOR *);
void func_00204F78(ACTOR *, SOURCE *, int);
void func_001E67B8(SOURCE *, float, qword *, qword *);
EFFECT *func_001FBB90(int);
void func_001FB2F8(EFFECT *, SOURCE *);
void func_00258040(EFFECT **, EFFECT *);
void func_002046F0(ACTOR *, SOURCE *);
void func_00204CA8(ACTOR *);
void func_002046B0(ACTOR *, qword);
void func_0014E328(MANAGER *, int, CAMERA *, ACTOR *, void *);
void func_0014E4C8(MANAGER *, int, int, CAMERA *, ACTOR *, void *);
void func_0018E540(ACTOR *, int);
void func_001CCAC0(ATTACH *, int);
void func_001CD2C0(void *);
void func_001FB098(EFFECT *, float);
void func_00140C88(ACTOR *, void *);
void func_00194118(CONTEXT *);
qword ConvertAloPos(void *, void *, qword);
void func_0020BD68(PROGRAM_ITEM *);
void func_00130658(ANIMATION **);
void func_001F15B8(COMPONENT *);
void func_001305B8(CLIP *, ACTOR *, float, float, int, ANIMATION **);
void func_001F1448(PROGRAM *, ACTOR *, int, int, COMPONENT *);
void func_0018C120(ACTOR *, int);
}

static inline void SourceState(SOURCE *p, int state)
{
    VTE &v = p->vt[3];
    ((void (*)(void *, int, void *, void *, void *, void *))v.fn)((char *)p + v.delta, state, 0, 0, 0, 0);
}
static inline void Notify(LO *p, int slot) { VTE &v = p->vt[slot]; ((void (*)(void *))v.fn)((char *)p + v.delta); }
static inline void Notify(ACTOR *p, int slot) { VTE &v = p->vt[slot]; ((void (*)(void *))v.fn)((char *)p + v.delta); }
static inline void SetVector(ACTOR *p, int slot, qword q) { VTE &v = p->vt[slot]; ((void (*)(void *, qword))v.fn)((char *)p + v.delta, q); }
static inline void SetPointer(ACTOR *p, int slot, void *pv) { VTE &v = p->vt[slot]; ((void (*)(void *, void *))v.fn)((char *)p + v.delta, pv); }
// bit 0 set: the word holds the flags itself; otherwise it points to them
static inline void AddFlags(uint *pflags, uint bits)
{
    uint w = *pflags;
    if ((w ^ 1) & 1)
        *(uint *)w |= bits;
    else
        *pflags = w | bits;
}

// The never-chosen "V" alternative is allocation-only: the word stays a MEM until reload (as in func_001911A8).
static inline VU_FLOAT VuFloat(const float &g) { VU_FLOAT v; __asm__("qmtc2.ni %1,%0" : "=j,j"(v.q) : "r,V"(*(const int *)&g)); return v; }

static inline bool Less(int n, int limit) { return n < limit; }   // as in func_00195FB8: keeps the two range tests apart
static inline float X(qword q) { float f; __asm__("mtc1 %1,%0" : "=f"(f) : "r"(q)); return f; }
// The vector is moved to a GPR once (gcc's own qmfc2 for the "r" inputs) and its x word is stored straight from there:
// no C construct gives a 32-bit store of the low word of a TImode GPR (unions go through the stack, casts sign-extend).
// Same StoreX/GetY/GetZ helpers as the matched func_001E9AC0.cpp.
static inline void SetX(float *pg, qword q) { __asm__("sw %1,%0" : "=m"(*pg) : "r"(q)); }
static inline float GetY(qword q) { float f; __asm__("prot3w %0,%1" : "=r"(f) : "r"(q)); return f; }
static inline float GetZ(qword q) { float f; __asm__("pextuw %0,$zero,%1" : "=r"(f) : "r"(q)); return f; }

static inline qword Sub(qword a, qword b) { qword r; __asm__("vsub.xyzw %0,%1,%2" : "=j"(r) : "j"(a), "j"(b)); return r; }
// a + b * g
static inline qword AddScaled(qword a, qword b, VU_FLOAT g)
{
    qword r;
    __asm__("vadda.xyzw $ACC,%1,$vf0\n\tvmaddx.xyzw %0,%2,%3x" : "=j"(r) : "j"(a), "j"(b), "j"(g.q));
    return r;
}
static inline qword Dot(qword a, qword b)
{
    qword d, one;
    __asm__("vmul.xyz %0,%2,%3\n\tvaddw.x %1,$vf0,$vf0w\n\tvadday.x $ACC,%0,%0y\n\tvmaddz.x %0,%1,%0z" : "=j"(d), "=j"(one) : "j"(a), "j"(b));
    return d;
}
static inline qword Length(qword v)
{
    qword d, one, len;
    __asm__("vmul.xyz %0,%2,%2\n\tvaddw.x %1,$vf0,$vf0w\n\tvadday.x $ACC,%0,%0y\n\tvmaddz.x %0,%1,%0z" : "=j"(d), "=&j"(one) : "j"(v));
    __asm__("vsqrt Q,%1x\n\tvwaitq\n\tvaddq.x %0,$vf0,Q" : "=j"(len) : "j"(d));
    return len;
}
// vdiv/vwaitq and vmulq as separate ops: the divide arm then does not start with a set of the result, so jump.c
// leaves the fallback copy on its own path (the target's lq before the branch and qmtc2 in the delay slot)
static inline qword Unit(qword v, qword len)
{
    qword r;
    __asm__ __volatile__("vdiv Q,$vf0w,%0x\n\tvwaitq" : : "j"(len));
    __asm__("vmulq.xyzw %0,%1,Q" : "=j"(r) : "j"(v));
    return r;
}

static inline qword Normalize(qword v, qword fallback)
{
    qword len = Length(v);
    if (X(len) < 0.0001f)
        return fallback;
    return Unit(v, len);
}
static inline void Normalize(qword *pv) { *pv = Normalize(*pv, D_002A2DA0); }
static inline qword Transform(const M3 &m, qword q)
{
    qword r;
    __asm__("vmulax.xyz $ACC,%1,%4x\n\tvmadday.xyz $ACC,%2,%4y\n\tvmaddz.xyz %0,%3,%4z"
            : "=j"(r) : "j"(m.row[0]), "j"(m.row[1]), "j"(m.row[2]), "j"(q));
    return r;
}

extern "C" void func_00192BD8(CONTEXT *p, int step)
{
    if (step == p->step)
        return;

    // leave the old step
    switch (p->step) {
    case 1:
        p->pactor->x1038 = -1;
        break;
    case -1:
        if (p->focus && FIsBasicDerivedFrom__FP5BASIC3CID(p->focus, 10)) {
            SOURCE *source = p->focus;
            func_001E6F60(source, func_001A62E8(p->pactor), 0, 0, 0, &p->param, &p->value);
        }
        switch (p->action) {
        case 1: {
            SOURCE *source = p->focus;
            source->actor = p->pactor;
            func_00183B80(source, 2);
            if (p->pactor == D_002E1E40)
                Notify(p->pactor->first, 9);
            break;
        }
        case 4: {
            SOURCE *source = p->focus;
            SourceState(source, 5);
            SubscribeLoObject(source, p->pactor);
            func_00204F78(p->pactor, source, 1);
            ACTOR *actor = p->pactor;
            actor->position = func_001A62E8(source);
            break;
        }
        case 3: {
            SOURCE *source = p->focus;
            U dir, tangent;
            SourceState(source, 5);
            SubscribeLoObject(source, p->pactor);
            func_00204F78(p->pactor, source, 1);
            p->index = -1;
            func_001E67B8(source, p->param, &p->pactor->position, &tangent.q);
            dir.i[2] = 0;
            Normalize(&dir.q);
            qword normal = dir.q;   // read before the call: a VU pseudo spilled to 0x50
            p->reversed = X(Dot(normal, func_001A6318(p->pactor)->row[0])) < D_002BF63C;
            break;
        }
        case 2: {
            EFFECT *effect = func_001FBB90(0x5C);
            func_001FB2F8(effect, 0);
            func_00258040(&D_002E1E40->effect, effect);
            break;
        }
        case 11:
            if (p->focus)
                SubscribeLoObject(p->focus, p->pactor);
            break;
        case 12:
            if (p->focus)
                func_002046F0(p->pactor, p->focus);
            func_00204CA8(p->pactor);
            break;
        }
        p->previous = -1;
        p->pactor->tStamp = D_002DDED0.t;
        break;
    case 7:
        if (Less(step, 11) && !Less(step, 9)) {
            SOURCE *source = p->focus;
            if (source->other) {
                qword pos = func_001A62E8(source->other);
                SetVector(p->pactor, 29, Sub(pos, p->pactor->posOffset));
                p->pactor->motion = AddScaled(pos, D_002A2DC0, VuFloat(p->pactor->height));
                func_002046B0(p->pactor, func_001A6318(source->other)->row[0]);
                SetPointer(p->pactor, 30, p->pactor->local);
                SetVector(p->pactor, 35, D_002A2DD0);
                SetVector(p->pactor, 36, D_002A2DD0);
            }
            func_0014E328(&D_002DDF40, 10, (CAMERA *)((char *)&D_002DDF40 + 0x4C0), p->pactor, p->pactor->anchor);
        }
        break;
    case 9:
    case 10:
        if (!(Less(step, 11) && !Less(step, 9)))
            func_0014E4C8(&D_002DDF40, 2, 10, (CAMERA *)((char *)&D_002DDF40 + 0x4C0), 0, 0);
        break;
    }

    // enter the new one
    CLIP *clip = 0;
    PROGRAM *script = 0;
    int flags = 1;
    int special = 0;
    float blend = 0.0f;
    float speed = 1.0f;
    switch (step) {
    case -1:
        p->fInhibit = 0;
        switch (p->action) {
        case 1:
            func_00183B80(p->focus, 0);
            if (p->pactor == D_002E1E40)
                Notify(p->pactor->first, 8);
            break;
        case 3:
        case 4: {
            SOURCE *source = p->focus;
            SourceState(source, 6);
            UnsubscribeLoObject(source, p->pactor);
            func_00204F78(p->pactor, source, 0);
            break;
        }
        case 6:
            if (!p->pactor->x18D8)
                func_0018E540(p->pactor, -1);
            break;
        case 11:
            if (p->focus)
                UnsubscribeLoObject(p->focus, p->pactor);
            p->pactor->x130C = 0;
            p->pactor->x9BC = -1;
            break;
        case 5:
        case 9:
            func_001CCAC0(&p->attach, -1);
            break;
        case 12:
            p->pactor->x1318 = -1;
            func_001CD2C0(p->pactor->secondary);
            if (p->pactor == D_002E1E40)
                p->pactor->x1BCC = 0;
            break;
        case 2:
            if (D_002E1E40->effect) {
                func_001FB098(D_002E1E40->effect, 0.0f);
                func_00258040(&D_002E1E40->effect, 0);
            }
            break;
        }
        if (p->pactor->focus) {
            if (p->pactor->focus == p->aux)
                func_00140C88(p->pactor, 0);
            else if (p->pactor->focus == p->setting->focus)
                func_00140C88(p->pactor, 0);
        }
        // store order: the last uses of -1 and 0.0f (tag, param) are scheduled first, the rest stay in this order
        p->pactor->x1038 = -1;
        p->action = -1;
        p->step = -1;
        p->focus = 0;
        p->setting = 0;
        p->param = p->value = 0.0f;
        p->changed = 0;
        p->tag = -1;
        break;
    case 0:
    case 5:
        switch (p->action) {
        case 0:
            clip = p->clip174;
            flags = 9;
            break;
        case 13:
            clip = step == 0 ? p->clip510 : p->clip174;
            flags = 9;
            break;
        case 2:
            clip = p->clip514;
            flags = 9;
            break;
        case 1:
            script = p->script230;
            flags = 1;
            break;
        case 4:
            clip = p->clip234;
            flags = 9;
            special = 7;
            break;
        case 3:
            if (p->focus->enabled) {
                CLIP *override = p->overrideClip;
                if (override) {
                    clip = override;
                    p->gain318 = p->gain31C = 0.0f;
                } else
                    clip = p->clip238;
            } else
                clip = p->clip238;
            flags = 9;
            special = 31;
            break;
        case 6:
        case 7:
            clip = p->clip320;
            flags = 9;
            break;
        case 5:
        case 9:
            p->attach.focus = p->focus;
            speed = 0.0f;
            func_001CCAC0(&p->attach, 0);
            clip = p->attach.clip;
            blend = clip->duration * 0.5f;
            break;
        case 10:
            clip = p->pactor->pjoy->amount >= D_002BF640 ? p->clip4F0 : p->clip4F4;
            break;
        case 11:
            if (p->tag == 21) {
                clip = D_002F7900->specialClip;
                if (clip)
                    break;
            }
            clip = p->clip4F8;
            break;
        case 12:
            clip = p->clip50C;
            break;
        }
        p->pactor->velocity = D_002A2DD0;
        break;
    case 1:
        switch (p->action) {
        case 0:
            clip = p->clip170;
            if (!p->pactor->focus) {
                if (FIsBasicDerivedFrom__FP5BASIC3CID(p->focus, 12)) {
                    SOURCE *source = p->focus;
                    if (source->actor)
                        func_00140C88(p->pactor, source->actor);
                    else {
                        func_00194118(p);
                        p->aux->target = source;
                        func_00140C88(p->pactor, p->aux);
                    }
                } else if (FIsBasicDerivedFrom__FP5BASIC3CID(p->focus, 34)) {
                    SOURCE *source = p->focus;
                    if (source->owner)
                        func_00140C88(p->pactor, source->owner);
                    else {
                        func_00194118(p);
                        p->aux->enabled = 1;
                        AUX *aux = p->aux;
                        aux->position = ConvertAloPos(0, p->aux, func_001A62E8(source));
                        p->aux->value = source->parameter;
                        func_00140C88(p->pactor, p->aux);
                    }
                } else if (FIsBasicDerivedFrom__FP5BASIC3CID(p->focus, 38)) {
                    SOURCE *source = p->focus;
                    if (source->owner)
                        func_00140C88(p->pactor, source->owner);
                    else {
                        func_00194118(p);
                        qword v = Transform(*func_001A6318(source), D_002BF650);
                        SetX(&p->aux->data->x, v);
                        p->aux->data->y = GetY(v);
                        p->aux->data->z = GetZ(v);
                        func_00140C88(p->pactor, p->aux);
                    }
                }
            }
            break;
        case 13:
            clip = p->clip170;
            break;
        }
        flags = 9;
        break;
    case 4:
        switch (p->action) {
        case 3:
            clip = p->program240.clip;
            func_0020BD68(&p->program240);
            break;
        case 1:
            clip = p->program180.clip;
            func_0020BD68(&p->program180);
            break;
        case 2:
            clip = p->pactor->program.clip;
            func_0020BD68(&p->pactor->program);
            break;
        }

        flags = 9;
        break;
    case 3:
        clip = p->clip17C;
        p->pactor->velocity = D_002A2DD0;
        break;
    case 2:
        clip = p->clip178;
        p->pactor->velocity = D_002A2DD0;
        break;
    case 7: {
        SOURCE *source = p->focus;
        clip = p->clip324;
        if (source->other) {
            M3 basis = *func_001A6318(source->other);
            func_002046B0(p->pactor, basis.row[0]);
        }
        p->pactor->velocity = D_002A2DD0;
        SourceState(source, 5);
        break;
    }
    case 8:
        clip = p->clip328;
        p->pactor->velocity = D_002A2DD0;
        SourceState(p->focus, 6);
        break;
    case 9:
        script = p->script32C;
        if (p->step == 7) {
            AddFlags(&p->pactor->flags, 0x10);
            if (p->pactor == D_002E1E40) {
                AddFlags(&p->pactor->first->flags, 0x10);
                AddFlags(&D_002E1E40->second->flags, 0x10);
            }
        }
        p->pactor->velocity = D_002A2DD0;
        break;
    case 10:
        clip = p->clip430;
        p->pactor->x1038 = -1;
        flags = 9;
        p->pactor->velocity = D_002A2DD0;
        break;
    }
    p->step = step;
    p->tStep = D_002DDED0.t;
    if (step != -1) {
        func_00130658(&p->pactor->animation);
        func_001F15B8(&p->pactor->component);
        func_001305B8(clip, p->pactor, blend, speed, flags, &p->pactor->animation);
        func_001F1448(script, p->pactor, -1, flags, &p->pactor->component);
        Notify(p->pactor, 90);
        if (FIsBasicDerivedFrom__FP5BASIC3CID(p->pactor, 53))
            func_0018C120(p->pactor, special);
    }
}
