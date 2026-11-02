typedef int qword __attribute__((mode(TI)));
struct PL { char pad[0x18]; float g18; float g1C; };
struct O;
struct TABLE;
struct ASEGA {char pad[0x14];struct TABLE*table;struct O*owner;float time;char pad20[0x20];float speed40,scale44,factor48,factor4C;};
struct BBOX {float min[4],max[4];char pad20[0x40];struct O*owner;};
struct G;
struct Q { char pad[0xEC]; struct BBOX *xEC; };
struct CLOCK { float t; float dt; };
extern struct CLOCK D_002DDED0;
extern struct G D_003DD490;
extern float D_002C08E4;
extern qword D_002C08F0;
extern qword D_002A2DE0;
struct P {
    char pad0[0x28]; unsigned long x28; char pad1[0xEC - 0x30]; struct BBOX *xEC; char pad1b[0x150 - 0xF0]; struct PL *pl;
    char pad2[0x190 - 0x154]; qword v190; char pad3[0x2C0 - 0x1A0]; char x2C0[0x4C0 - 0x2C0]; char x4C0[0xDF0 - 0x4C0];
    char xDF0[0x1030 - 0xDF0]; struct ASEGA *p1030; void *p1034; char pad4[0x1320 - 0x1038]; char x1320[0x13D0 - 0x1320];
    int x13D0; char pad5[0x1430 - 0x13D4]; struct Q *p1430; float g1434; char pad6[0x1540 - 0x1438]; char x1540[0x190C - 0x1540];
    struct O *x190C; char pad7[0x1AC8 - 0x1910]; float g1AC8; void *x1ACC;
};
void UpdateStepplayer(struct P *p);
int FAsegaRetracted(struct O *p);
void func_001306A8(struct O **pp);
void GatherJtGrabCandidates__FP2SWP10STEPPLAYERUi(struct G *pg, struct P *p, int n);
void func_001B5038(struct P *p);
void UpdateMurrayStateMachine(struct P *p);
int func_001430C8(struct P *p);
void SetSmaMask(void *p, int n);
void func_00204660(struct P *p, float g);
qword func_00204E38(struct P *p, struct ASEGA *pl, float g);
void UpdateGaitBlend(void *p, float g);
void UpdateGaitFootsteps(void *p);
void UpdateJtActiveJump(void *p);
void UpdateJtActiveHide(void *p);
void func_001B8B70(struct P *p);
void func_001B90B8(struct P *p);
void SetAsegaSpeed__FP5ASEGAf(struct ASEGA *p,float g);
void func_001B82A0(struct P *p);
void UpdateJtActiveZap(void *p);
int func_00133D08(struct G *pg,struct BBOX *a,struct BBOX *b);
void RecalcSwOxfFilterForObject(struct G *pg, struct P *p);
void SetActPositionGoal(void *p, qword v);
static inline qword Bcast(float g) { qword a; __asm__("qmtc2.ni %1, %0" : "=j"(a) : "r"(g)); return a; }
static inline void Neg(struct P *p, qword v)
{
    union { qword q; float f[4]; } u;
    qword b = Bcast(-1.0f);
    u.q = b;
    __asm__("vmulx.xyzw %0, %1, %2x" : "=j"(p->v190) : "j"(v), "j"(u.q));
}
static inline void Neg2(struct P *p, qword v)
{
    struct ASEGA *pl = p->p1030;
    volatile qword u; /* scalar stack copy of the -1 broadcast (target stores it to 0(sp) last) */
    qword b = Bcast(-1.0f); qword r;
    __asm__("vmulx.xyzw %0, %1, %2x" : "=j"(r) : "j"(v), "j"(b));
    p->v190 = r;
    u = b;
    SetAsegaSpeed__FP5ASEGAf(pl,p->pl->g1C);
}
#define FIELD_OFFSET(t,m) ((unsigned int)&(((t*)0x10000)->m)-0x10000)
typedef char Offset_0[FIELD_OFFSET(struct PL,g18)==24?1:-1];
typedef char Offset_1[FIELD_OFFSET(struct PL,g1C)==28?1:-1];
typedef char Offset_2[FIELD_OFFSET(struct ASEGA,table)==20?1:-1];
typedef char Offset_3[FIELD_OFFSET(struct ASEGA,owner)==24?1:-1];
typedef char Offset_4[FIELD_OFFSET(struct ASEGA,time)==28?1:-1];
typedef char Offset_5[FIELD_OFFSET(struct ASEGA,speed40)==64?1:-1];
typedef char Offset_6[FIELD_OFFSET(struct ASEGA,scale44)==68?1:-1];
typedef char Offset_7[FIELD_OFFSET(struct ASEGA,factor48)==72?1:-1];
typedef char Offset_8[FIELD_OFFSET(struct ASEGA,factor4C)==76?1:-1];
typedef char Offset_9[FIELD_OFFSET(struct BBOX,min)==0?1:-1];
typedef char Offset_10[FIELD_OFFSET(struct BBOX,max)==16?1:-1];
typedef char Offset_11[FIELD_OFFSET(struct BBOX,owner)==96?1:-1];
typedef char Offset_12[FIELD_OFFSET(struct Q,xEC)==236?1:-1];
typedef char Offset_13[FIELD_OFFSET(struct CLOCK,t)==0?1:-1];
typedef char Offset_14[FIELD_OFFSET(struct CLOCK,dt)==4?1:-1];
typedef char Offset_15[FIELD_OFFSET(struct P,x28)==40?1:-1];
typedef char Offset_16[FIELD_OFFSET(struct P,xEC)==236?1:-1];
typedef char Offset_17[FIELD_OFFSET(struct P,pl)==336?1:-1];
typedef char Offset_18[FIELD_OFFSET(struct P,v190)==400?1:-1];
typedef char Offset_19[FIELD_OFFSET(struct P,x2C0)==704?1:-1];
typedef char Offset_20[FIELD_OFFSET(struct P,x4C0)==1216?1:-1];
typedef char Offset_21[FIELD_OFFSET(struct P,xDF0)==3568?1:-1];
typedef char Offset_22[FIELD_OFFSET(struct P,p1030)==4144?1:-1];
typedef char Offset_23[FIELD_OFFSET(struct P,p1034)==4148?1:-1];
typedef char Offset_24[FIELD_OFFSET(struct P,x1320)==4896?1:-1];
typedef char Offset_25[FIELD_OFFSET(struct P,x13D0)==5072?1:-1];
typedef char Offset_26[FIELD_OFFSET(struct P,p1430)==5168?1:-1];
typedef char Offset_27[FIELD_OFFSET(struct P,g1434)==5172?1:-1];
typedef char Offset_28[FIELD_OFFSET(struct P,x1540)==5440?1:-1];
typedef char Offset_29[FIELD_OFFSET(struct P,x190C)==6412?1:-1];
typedef char Offset_30[FIELD_OFFSET(struct P,g1AC8)==6856?1:-1];
typedef char Offset_31[FIELD_OFFSET(struct P,x1ACC)==6860?1:-1];
typedef char Size_0[sizeof(struct PL)==32?1:-1];
typedef char Size_1[sizeof(struct ASEGA)==80?1:-1];
typedef char Size_2[sizeof(struct BBOX)==100?1:-1];
typedef char Size_3[sizeof(struct Q)==240?1:-1];
typedef char Size_4[sizeof(struct CLOCK)==8?1:-1];
typedef char Size_5[sizeof(struct P)==6864?1:-1];
typedef char Size_6[sizeof(qword)==16?1:-1];
typedef char Size_7[sizeof(unsigned long)==8?1:-1];
typedef char Size_8[sizeof(float)==4?1:-1];
typedef char Size_9[sizeof(int)==4?1:-1];
typedef char Size_10[sizeof(void*)==4?1:-1];
/** @sly2-annot addr=0x1B50B0
 * Purpose: MURRAY vt16 Update: STEPPLAYER Update, gather grab candidates (0x194A40 g_sw,p,0xF8FA), state
 *   machine, per-state locomotion (blends x1320/x1540 driven by stick), root-motion velocities, ground
 *   effects, clears recently-thrown object p1430 once bounding boxes separate
 * Info: proposed name UpdateMurray (T1); class MURRAY slot16; TU murray
 * Confidence: high [T1 0.80] - evidence: static analysis; sources: r5b_rest
 * See: docs/reference/functions/player.md#murray-0x1b4728-0x1b9f80
 * @end-sly2-annot */
void UpdateMurray(struct P *p)
{
    UpdateStepplayer(p);
    if (!((int)(p->x28 >> 60) & 1))
        return;
    if (p->x190C && FAsegaRetracted(p->x190C))
        func_001306A8(&p->x190C);
    GatherJtGrabCandidates__FP2SWP10STEPPLAYERUi(&D_003DD490, p, 0xF8FA);
    func_001B5038(p);
    UpdateMurrayStateMachine(p);
    switch (p->x13D0) {
    case 0:
        SetSmaMask(p->p1034, func_001430C8(p));
        break;
    case 1:
        func_00204660(p, p->pl->g18);
        UpdateGaitBlend(p->x1320, 1.0f);
        UpdateGaitFootsteps(p->x1320);
        break;
    case 10:
        func_00204660(p, p->pl->g18);
        UpdateGaitBlend(p->x1540, p->pl->g1C);
        UpdateGaitFootsteps(p->x1540);
        break;
    case 2:
        UpdateJtActiveJump(p->x2C0);
        break;
    case 13:
        UpdateJtActiveHide(p->x4C0);
        break;
    case 11:
        func_001B8B70(p);
        break;
    case 12:
        func_001B90B8(p);
        break;
    case 17:
        Neg(p, func_00204E38(p, p->p1030, p->p1030->time));
        break;
    case 18:
        Neg2(p, func_00204E38(p, p->p1030, p->p1030->time));
        func_00204660(p, p->pl->g18);
        break;
    }
    func_001B82A0(p);
    UpdateJtActiveZap(p->xDF0);
    if (p->p1430 && p->g1434 + 0.1f < D_002DDED0.t) {
        if (!p->p1430->xEC || !func_00133D08(&D_003DD490, p->xEC, p->p1430->xEC))
            p->p1430 = 0;
        if (!p->p1430)
            RecalcSwOxfFilterForObject(&D_003DD490, p);
    }
    if (p->x1ACC) {
        if (D_002DDED0.t - p->g1AC8 < D_002C08E4)
            SetActPositionGoal(p->x1ACC, D_002C08F0);
        else
            SetActPositionGoal(p->x1ACC, D_002A2DE0);
    }
}
