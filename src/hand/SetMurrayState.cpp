// SetMurrayState(p, next): main state transition of the animated actor (state at +0x13D0).
// Exit work for the old state, then entry work for `next` (clip/template selection), then the shared
// "ApplyAseg" tail that restarts the animation player (+0x1030) and the component (+0x1034).
// Lane 4 (Claude agent): hacks of the earlier drafts (xori/mtc1/addiu asm, 6 pins) are gone; this has ONE pin
// (the zero-vector temp in case 7) and one VU vsub.
typedef int qword __attribute__((mode(TI)));
union U { qword q; float f[4]; int i[4]; };
struct VE { short delta, pad; void (*fn)(); };
struct OBJ { char pad[12]; VE *vt; };
struct ASEG { char pad[0x74]; float duration; };
struct ASEGA { char pad[0x14]; ASEG *clip; char pad18[4]; float time; char pad20[0x40 - 0x20]; float rate; };
struct COMPONENT;
struct ENTRY { float value; int kind, key, pad; };
struct P {
    char pad0[12];
    VE *vt;
    char pad10[72];
    void *first;            // 0x58
    void *second;           // 0x5C
    char pad60[304];
    U velocity;             // 0x190
    float phase;            // 0x1A0
    char pad1a4[804];
    int submode;            // 0x4C8
    char pad4cc[2908];
    void *template0;        // 0x1028
    void *template16;       // 0x102C
    ASEGA *asega;           // 0x1030
    COMPONENT *component;   // 0x1034
    char pad1038[84];
    int retain;             // 0x108C
    char pad1090[660];
    ASEG *clip1;            // 0x1324
    char pad1328[168];
    int state;              // 0x13D0
    float time;             // 0x13D4
    char pad13d8[8];
    ASEG *clip3;            // 0x13E0
    char pad13e4[4];
    int alternate3;         // 0x13E8
    char pad13ec[32];
    ASEG *clip7;            // 0x140C
    char pad1410[16];
    U goal;                 // 0x1420
    char pad1430[8];
    ASEG *clip8;            // 0x1438
    int alternate8;         // 0x143C
    ASEG *clip4a;           // 0x1440
    ASEG *clip4b;           // 0x1444
    char pad1448[4];
    int clipKind;           // 0x144C
    char pad1450[208];
    void *template6;        // 0x1520
    char pad1524[12];
    int marker;             // 0x1530
    char pad1534[16];
    ASEG *clip10;           // 0x1544
    char pad1548[980];
    OBJ *object15;          // 0x191C
    int stamp15;            // 0x1920
    char pad1924[300];
    OBJ *firstCleanup;      // 0x1A50
    OBJ *secondCleanup;     // 0x1A54
    char pad1a58[136];
    ASEG *extra7;           // 0x1AE0
    ASEG *extra8;           // 0x1AE4
    ASEG *extra3;           // 0x1AE8
    ASEG *clip21;           // 0x1AEC
};
extern "C" {
struct CLOCK { float t, dt; };
extern CLOCK D_002DDED0;
extern U D_002A2DD0;
extern int D_002DD5BC;
void func_002303A0(P *, void *, int);
void func_00142880(P *);
void SetMurrayFlopSubstate(P *, int);
void func_0015C150(OBJ *);
void func_00195FB8(void *, int, int);
void SetJtGrabKind(void *, int);
void SetJtHurtState(void *, int);
void SetMurrayAttack(P *, int);
void func_001B8D30(P *, int);
void SetMurrayLiftSubstate(P *, int);
void SetMurrayCarrySubstate(P *, int);
void func_001B5D30(P *, int);
void SetJtthrowState(void *, int);
void SetSoCnstrForce(P *, int);
void func_0019EAB0(void *, int);
int func_001F0400(void *, int);
int func_001430C8(P *);
void func_00142868(P *, int);
void func_0020BD68(void *);
void SetJtModeState__FP6JTMODEi(void *, int);
int func_001DAEA8(int);
void ChooseMurrayThrowTarget(P *);
qword func_001A62E8(P *);
void SetStepRadTargetVec(P *, qword);
void func_00130658(ASEGA **);
void ReleaseSma(COMPONENT **);
void func_001305B8(ASEG *, P *, float, float, int, ASEGA **);
void ReplaceSmaEx(void *, P *, int, int, int, COMPONENT **);
ENTRY *func_0012F938(ASEG *, int);
void func_00131A90(ASEGA *, float);
}
static inline void *Block(P *p, int offset) { return (void *)((char *)p + offset); }
// virtual call through slot `index` of an object whose vptr is at +0xC
static inline void Call(OBJ *p, int index) { VE &v = p->vt[index]; ((void (*)(void *))v.fn)((char *)p + v.delta); }
static inline void SetVelocity(P *p, qword q) { VE &v = p->vt[35]; ((void (*)(void *, qword))v.fn)((char *)p + v.delta, q); }
extern "C" void SetMurrayState(P *p, int next)
{
    if (next == p->state)
        return;
    // leave the old state
    switch (p->state) {
    case 0:
        func_002303A0(p, p->first, 2); func_002303A0(p, p->second, 2); func_00142880(p); break;
    case 16: case 17: case 18:
        func_002303A0(p, p->first, 2); func_002303A0(p, p->second, 2); break;
    case 3: SetMurrayFlopSubstate(p, -1); break;
    case 2: {
        if (!p->retain) {
            OBJ *object = 0;
            switch (next) {
            case 0: case 1: case 4: case 5: case 9: case 11: case 12: object = p->firstCleanup; break;
            case 2: case 3: case 6: case 7: case 8: case 10: break;
            case 13: {
                switch (p->submode) { case 1: break; case 3: case 4: object = p->secondCleanup; break; default: object = p->firstCleanup; break; }
                break;
            }
            }
            if (object) { Call(object, 8); func_0015C150(object); Call(object, 9); }
        }
        func_00195FB8(Block(p, 0x2C0), -1, 0); break;
    }
    case 13: SetJtGrabKind(Block(p, 0x4C0), -1); break;
    case 14: SetJtHurtState(Block(p, 0xDF0), -1); break;
    case 11: SetMurrayAttack(p, -1); break;
    case 12: func_001B8D30(p, -1); break;
    case 4: SetMurrayLiftSubstate(p, -1); break;
    case 5: SetMurrayCarrySubstate(p, -1); break;
    case 9: func_001B5D30(p, -1); break;
    case 15: Call(p->object15, 9); SetJtthrowState(Block(p, 0x1910), -1); SetSoCnstrForce(p, 0); break;
    case 1: case 10: p->phase = p->asega->time / p->asega->clip->duration; break;
    case 6: p->marker = 0; break;
    case 20: func_0019EAB0(Block(p, 0xF50), -1); break;
    case 7: case 8: case 19: break;
    }
    int old = p->state;
    p->state = next;
    ASEG *clip = 0; void *source = 0; int flags = 1, id = -1, mode = 4; void *activation;
    float duration = 0.0f;
    p->time = D_002DDED0.t;
    // enter the new state
    switch (next) {
    case 0:
        func_002303A0(p, p->first, 1); func_002303A0(p, p->second, 1);
        source = p->template0; p->velocity = D_002A2DD0;
        if (old != 2) { id = (old != 10) ? 0x175 : 0x176; } else id = 0x178;
        if (func_001F0400(source, id) < 0) id = -1;
        mode = func_001430C8(p); func_00142868(p, 1); break;
    case 1: clip = p->clip1; activation = Block(p, 0x1320); goto activate;
    case 14: SetJtModeState__FP6JTMODEi(Block(p, 0xF00), -1); break;
    case 3: { SetMurrayFlopSubstate(p, 0); ASEG *a = p->extra3; if (a && p->alternate3) clip = a; else clip = p->clip3; break; }
    case 5: SetMurrayCarrySubstate(p, 0); Call((OBJ *)p, 90); return;
    case 6: source = p->template6; break;
    case 4:
        switch (p->clipKind) { case 0: clip = p->clip4a; break; case 1: clip = p->clip4b; break; }
        p->velocity = D_002A2DD0; SetMurrayLiftSubstate(p, 0); break;
    case 7: {
        ASEG *a = p->extra7;
        if (a && func_001DAEA8(0x11)) clip = p->extra7; else clip = p->clip7;
        { register qword zero __asm__("$3") = D_002A2DD0.q; p->velocity.q = zero; }   // pin: the zero vector goes through $v1 here
        ChooseMurrayThrowTarget(p);
        U point = p->goal; qword current = func_001A62E8(p);
        qword difference;
        __asm__("vsub.xyzw %0,%0,%2" : "=j"(difference) : "0"(point.q), "j"(current));
        SetStepRadTargetVec(p, difference); break;
    }
    case 8: { ASEG *a = p->extra8; if (a && p->alternate8) clip = a; else clip = p->clip8; goto clear_velocity; }
    case 9: func_001B5D30(p, 0); Call((OBJ *)p, 90); return;
    case 10: clip = p->clip10; activation = Block(p, 0x1540);
    activate: flags = 9; func_0020BD68(activation); break;
    case 15:
        Call(p->object15, 8); p->stamp15 = D_002DD5BC; SetJtthrowState(Block(p, 0x1910), 0); SetSoCnstrForce(p, 1); SetVelocity(p, D_002A2DD0.q); return;
    case 16: source = p->template16;
    case 17: case 18:
        func_002303A0(p, p->first, 1); func_002303A0(p, p->second, 1);
    clear_velocity: p->velocity = D_002A2DD0; break;
    case 20: func_0019EAB0(Block(p, 0xF50), 0); return;
    case 21: { clip = p->clip21; U *velocity = &p->velocity; velocity->i[2] = 0; flags = 9; velocity->i[1] = 0; p->velocity.i[0] = 0; break; }
    case 2: case 11: case 12: case 13: case 19: break;
    }
    {
        ASEGA **animation = &p->asega; COMPONENT **component = &p->component; float position = 0.0f;
        func_00130658(animation); ReleaseSma(component);
        func_001305B8(clip, p, position, 1.0f, flags, animation);
        ReplaceSmaEx(source, p, id, flags, mode, component);
        if (clip || source) {
            if (clip) { if (duration == position) { ENTRY *e = func_0012F938(clip, 0x1AC); if (e) duration = e->value; } }
            if (duration != 0.0f) func_00131A90(p->asega, D_002DDED0.t + duration / p->asega->rate);
        }
    }
    Call((OBJ *)p, 90);
}
