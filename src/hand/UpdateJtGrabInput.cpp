// UpdateJtGrabInput: per-frame update of a hero's action context (the struct at actor+0x4C0; UpdateBentleyStateMachine calls it in
// its state 3). It dispatches on the context's action and loops while the action changed the step (and not to -1).
// Action 8 / step "other" probes the ground below the actor: a box query along the actor's up axis, a line test
// against every found object (any hit with a nonzero parameter aborts), then a sphere around the actor's world bounds
// is pruned against every object's BSP and turned into contacts; any contact aborts, otherwise step 8 is entered.
//
// Matching notes (wave-4 lane 8):
// - case 8 switches on p->step (the field, not the loop's local): gcse turns the load into a copy of the loop-top
//   load, which gives the `lw a1, step` in the dispatch branch's delay slot; the local itself is spilled.
// - ProbeGround's exits are plain returns out of a STACKSCOPE scope (out-of-line ctor/dtor 0x1AE478/0x1AE4A0);
//   loop.c moves the `{ FreeSwXpList(); return; }` block out of the contact loop into the default case.
// - case 2 has its own SetControl for the `<= 0` test: the shared SetControl then has no label in front of it, so
//   jump2's cross-jumping makes a new label there and does not merge the step-8 / case-11 anim tests afterwards.
// - GLength takes a const reference (gcse hash table size, i.e. the order of the &cso / &apso copies: s4 / s6).
typedef int qword __attribute__((mode(TI)));
typedef unsigned int uint;
typedef unsigned long ulong;

struct VTE { short delta, pad; void *fn; };
struct JOY { char pad0[0x1C]; float amount; char pad20[0x10]; uint grfbtn; };
struct SURF;
struct BSP { BSP *pbspNeg, *pbspPos; SURF *psurf; };
struct XP;
struct SW;
struct ASEGA;
struct GEOM { int cpos; qword *apos; };
struct SO {
    char pad0[0xC]; VTE *vt; ulong flags; char pad18[0x6C - 0x18]; int cbsp; char pad70[4]; BSP *absp;
    char pad78[0x90 - 0x78]; GEOM geomWorld;
};
// Line segment hit (0x60 bytes, as in func_001A93C8)
struct LSG { qword apos[2]; qword anormal[2]; float au[2]; int lsgk; void *apsurf[2]; void *pedge; int iiposSwap; LSG *plsgNext; };
struct ACTOR {
    char pad0[0xC]; VTE *vt; char pad10[0xEC - 0x10]; void *pvFilter; char padF0[0x150 - 0xF0]; JOY *pjoy;
    char pad154[0x240 - 0x154]; qword posOffset; char pad250[0x268 - 0x250]; SO *pso; char pad26C[0xF5C - 0x26C];
    ACTOR *ptarget; float gTarget; char padF64[0x1030 - 0xF64]; ASEGA *pasega; char pad1034[0x10EC - 0x1034]; int x10EC;
};
struct ATTACH { char pad0[0x34]; ACTOR *pactor; };
struct CONTEXT {
    ACTOR *pactor; float tStamp; int action; int step; float tHold; ACTOR *pactorFocus; char pad18[8]; float gParam;
    char pad24[0x440 - 0x24]; ATTACH attach; char pad478[0x500 - 0x478]; int fInhibit;
};
struct CLOCK { float t, dt; };
struct SWALLOC { int cb, cbAlign; };
struct STACKSCOPE { STACKSCOPE(); ~STACKSCOPE(); };   // 0x1AE478 / 0x1AE4A0(this, 2)
struct VU_FLOAT { qword q; };

extern "C" {
extern SW D_003DD490;                                   // the world
extern CLOCK D_002DDED0;                                // g_clock
extern uint D_002DFBC4;
extern qword D_002A2DC0;                                // (0, 0, 1)
extern float D_002BF5B0, D_002BF5B4, D_002BF5B8;
int GrfConsumePressed__5POCTLUif(JOY *, uint, float);
int GrfPressedWithin__5POCTLUif(JOY *, uint, float);
void ClearPoctlJoyPressed(JOY *, uint);
void func_00195A00(void *, int, int, ACTOR *);
void func_0019CC50(void *, int, int, int);
int FAsegaRetracted(ASEGA *);
void SetJtGrabKind(CONTEXT *, int);                     // set the step
void *func_001AE318(SWALLOC *);                         // stack allocation in the current STACKSCOPE
qword func_001A62E8(ACTOR *);                           // world position
int FFilterSoPair(void *, SO *);
void IntersectSwBoundingBox(SW *, qword, qword, int (*)(void *, SO *), void *, int *, SO ***);   // objects in a box
int func_0014C6A0(SO *, qword, qword, int, LSG *);      // line test against an object
void UpdateSoPartGeom(SO *);
qword VecMinArray(int, qword *);                      // min of points
qword VecMaxArray(int, qword *);                      // max of points
void func_001A9B28(SW *, SO *, SO *, int, BSP *, XP **);
int func_00118A88(BSP *, BSP *, float, qword, int *, BSP **);   // prune a BSP to a sphere
void func_001A8B50(SW *, SO *, SO *, int, BSP *, BSP *, XP **);
void func_001CD098(ATTACH *, ACTOR *);
int FIsBasicDerivedFrom__FP5BASIC3CID(ACTOR *, int);
int func_001423A0(ACTOR *);
int MpStepplayerCur(ACTOR *);
uint GrfbtnGadget(ACTOR *, int);
}
void FreeSwXpList(SW *, XP *);

static inline void SetControl(ACTOR *p, int n) { VTE &v = p->vt[88]; ((void (*)(void *, int))v.fn)((char *)p + v.delta, n); }
static inline int CanAct(ACTOR *p, int n) { VTE &v = p->vt[99]; return ((int (*)(void *, int))v.fn)((char *)p + v.delta, n); }
static inline void Notify(ACTOR *p, ACTOR *q) { VTE &v = p->vt[51]; ((void (*)(void *, ACTOR *))v.fn)((char *)p + v.delta, q); }
static inline void *PvAllocStack(int cb) { SWALLOC swalloc; swalloc.cb = cb; swalloc.cbAlign = 0x10; return func_001AE318(&swalloc); }

static inline VU_FLOAT VuFloat(float g) { VU_FLOAT v; __asm__("qmtc2.ni %1,%0" : "=j"(v.q) : "r"(g)); return v; }
// The never-chosen "V" alternative is allocation-only: the word stays a MEM until reload (as in func_0014F110).
static inline VU_FLOAT VuFloat(const float *pg) { VU_FLOAT v; __asm__("qmtc2.ni %1,%0" : "=j,j"(v.q) : "r,V"(*(const int *)pg)); return v; }
static inline qword QwAdd(qword a, qword b) { qword r; __asm__("vadd.xyzw %0,%1,%2" : "=j"(r) : "j"(a), "j"(b)); return r; }
static inline qword QwSub(qword a, qword b) { qword r; __asm__("vsub.xyzw %0,%1,%2" : "=j"(r) : "j"(a), "j"(b)); return r; }
static inline qword QwScale(qword a, VU_FLOAT g) { qword r; __asm__("vmulx.xyzw %0,%1,%2x" : "=j"(r) : "j"(a), "j"(g.q)); return r; }
// a * g0 + b * g1
static inline qword QwCombine(qword a, qword b, VU_FLOAT g0, VU_FLOAT g1)
{
    qword r;
    __asm__("vmulax.xyzw $ACC,%1,%2x\n\tvmaddx.xyzw %0,%3,%4x" : "=j"(r) : "j"(a), "j"(g0.q), "j"(b), "j"(g1.q));
    return r;
}
static inline float GFromVf(qword v) { float g; __asm__("qmfc2.ni %0,%1" : "=r"(g) : "j"(v)); return g; }
static inline float GLength(const qword &v)
{
    qword qw, one;
    __asm__("vmul.xyz %0,%2,%2\n\tvaddw.x %1,$vf0,$vf0w\n\tvadday.x $ACC,%0,%0y\n\tvmaddz.x %0,%1,%0z" : "=j"(qw), "=j"(one) : "j"(v));
    __asm__("vsqrt $Q,%0x\n\tvwaitq\n\tvaddq.x %0,$vf0,$Q" : "=j"(qw) : "0"(qw));
    return GFromVf(qw);
}
static inline int Min(int a, int b) { return a < b ? a : b; }
static inline int FWithin(qword a, qword b, float s) { return GLength(QwSub(a, b)) < s; }

static inline void ProbeGround(CONTEXT *p)
{
    STACKSCOPE scope;
    LSG alsg[8];
    int i;
    qword pos = func_001A62E8(p->pactor);
    qword posBase = QwAdd(pos, p->pactor->posOffset);
    qword posEnd = QwAdd(posBase, QwScale(D_002A2DC0, VuFloat(&D_002BF5B0)));
    int cso;
    SO **apso;
    IntersectSwBoundingBox(&D_003DD490, posEnd, posBase, FFilterSoPair, p->pactor->pvFilter, &cso, &apso);
    for (i = 0; i < cso; i++) {
        int clsg = func_0014C6A0(apso[i], posBase, posEnd, 8, alsg);
        LSG *plsg = alsg;
        for (int j = 0; j < clsg; j++)
            if (plsg[j].au[0] != 0.0f)
                return;
    }
    SO *psoSelf = p->pactor->pso;
    GEOM *pgeom = &psoSelf->geomWorld;
    int cbspMax = 0;
    UpdateSoPartGeom(psoSelf);
    qword posMin = VecMinArray(pgeom->cpos, pgeom->apos);
    qword posMax = VecMaxArray(pgeom->cpos, pgeom->apos);
    IntersectSwBoundingBox(&D_003DD490, posMin, posMax, FFilterSoPair, p->pactor->pvFilter, &cso, &apso);
    for (i = 0; i < cso; i++)
        if (cbspMax <= apso[i]->cbsp)
            cbspMax = apso[i]->cbsp;
    cbspMax = Min(cbspMax, 0x1555);
    BSP *absp = (BSP *)PvAllocStack(cbspMax * sizeof(BSP));
    qword posMid = QwCombine(posMin, posMax, VuFloat(0.5f), VuFloat(0.5f));
    float sRadius = GLength(QwSub(posMid, posMin));
    for (i = 0; i < cso; i++) {
        SO *pso = apso[i];
        XP *pxp = 0;
        if (pso->flags & (1UL << 49)) {
            func_001A9B28(&D_003DD490, pso, psoSelf, 0, 0, &pxp);
        } else {
            int cbspLeft = cbspMax;
            BSP *pbsp = absp;
            if (func_00118A88(pso->absp, (BSP *)0x7FFFFFFF, sRadius, posMid, &cbspLeft, &pbsp) == 2) {
                int cbsp = pbsp - absp;
                pbsp--;
                func_001A8B50(&D_003DD490, psoSelf, pso, cbsp, pso->absp, pbsp, &pxp);
            }
        }
        if (pxp) {
            FreeSwXpList(&D_003DD490, pxp);
            return;
        }
    }
    SetJtGrabKind(p, 8);
}

extern "C" void UpdateJtGrabInput(CONTEXT *p)
{
    JOY *pjoy = p->pactor->pjoy;
    int step;
    do {
        step = p->step;
        switch (p->action) {
        case 1:
            if (0.15f < D_002DDED0.t - p->tHold && GrfConsumePressed__5POCTLUif(pjoy, 0x40, 0.25f)) {
                func_00195A00((char *)p->pactor + 0x2C0, 7, 0, p->pactorFocus);
                break;
            }
            if (GrfPressedWithin__5POCTLUif(pjoy, 0x400, 0.15f) && CanAct(p->pactor, 3)) {
                ClearPoctlJoyPressed(pjoy, 0x400);
                func_0019CC50((char *)p->pactor + 0xF00, 0, 0, 0);
            }
            break;
        case 3:
        case 4:
            if (GrfConsumePressed__5POCTLUif(pjoy, 0x40, 0.1f)) {
                func_00195A00((char *)p->pactor + 0x2C0, 1, 0, 0);
                break;
            }
            if (GrfPressedWithin__5POCTLUif(pjoy, 0x400, 0.15f) && CanAct(p->pactor, 3)) {
                ClearPoctlJoyPressed(pjoy, 0x400);
                func_0019CC50((char *)p->pactor + 0xF00, 0, 0, 0);
            }
            break;
        case 7: {
            ACTOR *pactorFocus = p->pactorFocus;
            SetControl(p->pactor, 0);
            Notify(pactorFocus, p->pactor);
            break;
        }
        case 8:
            switch (p->step) {
            case 7:
                if (FAsegaRetracted(p->pactor->pasega))
                    SetJtGrabKind(p, 9);
                break;
            case 8:
                if (FAsegaRetracted(p->pactor->pasega))
                    SetControl(p->pactor, 0);
                break;
            default:
                ProbeGround(p);
                break;
            }
            break;
        case 9:
            if (FWithin(func_001A62E8(p->pactorFocus), func_001A62E8(p->attach.pactor), D_002BF5B4)) {
                func_001CD098(&p->attach, p->pactorFocus);
                SetControl(p->pactor, 0);
            }
            break;
        case 10:
            if (!p->pactor->pasega || FAsegaRetracted(p->pactor->pasega)) {
                SetControl(p->pactor, 0);
            } else if (D_002BF5B8 < D_002DDED0.t - p->tStamp) {
                if (0.0f < p->pactor->pjoy->amount || (p->pactor->pjoy->grfbtn & 0xD0)) {
                    SetControl(p->pactor, 0);
                    return;
                }
            }
            break;
        case 11:
        case 12:
            if (!p->pactor->pasega || FAsegaRetracted(p->pactor->pasega))
                SetControl(p->pactor, 0);
            break;
        case 5:
            if (FIsBasicDerivedFrom__FP5BASIC3CID(p->pactorFocus, 11) && func_001423A0(p->pactor)) {
                p->pactor->ptarget = p->pactorFocus;
                p->pactor->gTarget = p->gParam;
                SetControl(p->pactor, 8);
            }
            break;
        case 2:
            if (MpStepplayerCur(p->pactor) <= 0) {
                SetControl(p->pactor, 0);
                break;
            }
            if (p->pactor->x10EC == 4) {
                SetControl(p->pactor, 0);
                break;
            }
            if (D_002DFBC4 & 1)
                break;
            if (pjoy->grfbtn & GrfbtnGadget(p->pactor, 0x28))
                break;
            SetControl(p->pactor, 0);
            break;
        default:
            if (GrfConsumePressed__5POCTLUif(pjoy, 0x40, 0.1f)) {
                func_00195A00((char *)p->pactor + 0x2C0, 1, 0, 0);
                break;
            }
            if (GrfPressedWithin__5POCTLUif(pjoy, 0x400, 0.15f) && CanAct(p->pactor, 3) && (p->action == 0 || p->action == 13)) {
                ClearPoctlJoyPressed(pjoy, 0x400);
                func_0019CC50((char *)p->pactor + 0xF00, 0, 0, 0);
                break;
            }
            if (p->fInhibit)
                break;
            if (D_002DFBC4 & 1)
                break;
            if (pjoy->grfbtn & 0x20)
                break;
            SetControl(p->pactor, 0);
            break;
        }
    } while (p->step != step && p->step != -1);
}

#define FIELD_OFFSET(t, m) ((unsigned int)&(((t *)0x10000)->m) - 0x10000)
typedef char LSG_Size[sizeof(LSG) == 0x60 ? 1 : -1];
typedef char BSP_Size[sizeof(BSP) == 12 ? 1 : -1];
typedef char SO_absp_Offset[FIELD_OFFSET(SO, absp) == 0x74 ? 1 : -1];
typedef char SO_geomWorld_Offset[FIELD_OFFSET(SO, geomWorld) == 0x90 ? 1 : -1];
typedef char ACTOR_pso_Offset[FIELD_OFFSET(ACTOR, pso) == 0x268 ? 1 : -1];
typedef char ACTOR_x10EC_Offset[FIELD_OFFSET(ACTOR, x10EC) == 0x10EC ? 1 : -1];
typedef char CONTEXT_attach_Offset[FIELD_OFFSET(CONTEXT, attach) == 0x440 ? 1 : -1];
typedef char CONTEXT_fInhibit_Offset[FIELD_OFFSET(CONTEXT, fInhibit) == 0x500 ? 1 : -1];
