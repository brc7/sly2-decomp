// UpdateBhvfollow: camera/AI state update. Keys: `f = func_001459C0(p) != 0; if (f)` (stops loop.c hoisting constant 1), inline GClamp.
typedef int qword __attribute__((mode(TI)));
struct LM { float gMin, gMax; };
struct D { char pad[0x10]; void *p10; };
struct C { char pad[0xDC]; struct D *pd; };
struct PO { char pad[0xE8]; struct C *pc; char pad2[0x2DC - 0xEC]; int x2DC; char pad3[0x1030 - 0x2E0]; void *p1030; };
struct X { char pad[0x40]; int x40; };
struct SO;
struct P {
    char pad[0x10]; struct PO *p10; char pad1[4]; struct X *p18; char pad2[0x30 - 0x1C]; void *p30; char pad3[0x50 - 0x34];
    qword q50; int x60; char pad4[0x6C - 0x64]; float g6C; float g70; float g74; float g78; char pad5[0x90 - 0x7C]; struct SO *p90;
};
extern float D_002A4E34;
extern struct LM D_002BD330;
extern struct LM D_002BD338;
extern float D_002BD340;
void UpdateBhv(struct P *p);
int FBhvBusyAbove(struct P *p);
qword func_001A62E8(void *p);
struct SO *func_001450E0(struct P *p);
int func_001459C0(struct P *p);
int FCheckLm__FP2LMf(struct LM *plm, float g);
void SetBhvfollowState(struct P *p, int n);
int CidBasic(void *p);
int func_00145DA0(struct X *px);
int func_00146298(struct X *px);
int func_00146790(struct X *px);
void func_001457A8(struct P *p);
void SetAsegaSpeed__FP5ASEGAf(void *p, float g);

static __inline__ float LenXY(qword d)
{
    float g;
    qword t;
    __asm__("vmul.xy %0, %1, %1\n\tvaddy.x %0, %0, %0y" : "=j"(t) : "j"(d));
    __asm__("vsqrt Q, %0x\n\tvwaitq\n\tvaddq.x %0, $vf0, Q" : "+j"(t));
    __asm__("qmfc2.ni %0, %1" : "=r"(g) : "j"(t));
    return g;
}
static __inline__ qword Sub(qword a, qword b) { qword d; __asm__("vsub.xyzw %0, %1, %2" : "=j"(d) : "j"(a), "j"(b)); return d; }
static __inline__ float Len(qword d)
{
    qword t;
    float g;
    __asm__("vmul.xyz %0, %0, %0\n\tvaddw.x %1, $vf0, $vf0w\n\tvadday.x ACC, %0, %0y\n\tvmaddz.x %0, %1, %0z" : "+j"(d), "=j"(t));
    __asm__("vsqrt Q, %0x\n\tvwaitq\n\tvaddq.x %0, $vf0, Q" : "+j"(d));
    __asm__("qmfc2.ni %0, %1" : "=r"(g) : "j"(d));
    return g;
}
static __inline__ void GClamp(float *pg, float lo, float hi) { if (*pg < lo) *pg = lo; else if (hi < *pg) *pg = hi; }
static __inline__ int FFar(qword a, qword b) { float lim = D_002BD340; return lim < LenXY(Sub(a, b)); }

/** @sly2-annot addr=0x1453F0
 * Purpose: BHV194 slot 10: follow/escort another object (+0x30): distance bands switch states, re-path when
 *   far
 * Info: proposed name UpdateBhvfollow (T1); class BHVFOLLOW slot10; TU tu_144B28
 * Notes: v3 BHV class word (runtime6 P2: BHV194 = follow, BHV168 = walk-to-goal child): UpdateBhv194 ->
 *   UpdateBhvfollow | runtime6 P2: distance bands/leader +0x30 = JT confirmed (S62)
 * Confidence: mid (draft) [T1 0.60] - evidence: runtime (save-state RAM); sources: r2b_rest+runtime(class
 *   role)+runtime
 * See: docs/reference/functions/player.md#tu_144b28-0x144b28-0x1471c0
 * @end-sly2-annot */
void UpdateBhvfollow(struct P *p)
{
    struct PO *po;
    qword a, b;
    float gXY, g;
    struct SO *pso;
    void *pv30;
    UpdateBhv(p);
    po = p->p10;
    pv30 = p->p30;
    a = func_001A62E8(po);
    b = func_001A62E8(pv30);
    gXY = LenXY(Sub(a, b));
    pso = func_001450E0(p);
    if (pso) {
        qword c = func_001A62E8(pso);
        float dA = Len(Sub(c, a));
        float dB = Len(Sub(c, b));
        g = dA < dB ? dA : dB;
    } else {
        g = D_002A4E34;
    }
    while (!FBhvBusyAbove(p)) {
        int state = p->x60;
        int f;
        switch (state) {
        case 0:
            f = func_001459C0(p) != 0;
            if (f)
                SetBhvfollowState(p, 1);
            else if (FCheckLm__FP2LMf(&D_002BD330, g)) {
                p->p90 = pso;
                SetBhvfollowState(p, 2);
            }
            break;
        case 1:
            if (p->p10->x2DC == -1)
                SetBhvfollowState(p, 0);
            break;
        case 2:
            f = func_001459C0(p) != 0;
            if (f)
                SetBhvfollowState(p, 1);
            else if (!FCheckLm__FP2LMf(&D_002BD338, g))
                SetBhvfollowState(p, 0);
            else {
                switch (CidBasic(p->p10)) {
                case 0x36:
                    if (func_00145DA0(p->p18))
                        SetBhvfollowState(p, 0);
                    break;
                case 0x37:
                    if (func_00146298(p->p18))
                        SetBhvfollowState(p, 0);
                    break;
                case 0x35:
                    if (func_00146790(p->p18))
                        SetBhvfollowState(p, 0);
                    break;
                }
            }
            break;
        }
        if (p->x60 == state)
            break;
    }
    if (p->x60 == 0) {
        struct D *pd = p->p10->pc ? p->p10->pc->pd : 0;
        void *pv = pd ? pd->p10 : 0;
        if (!pv)
            p->p18->x40 = 1;
        if (FFar(p->q50, b))
            func_001457A8(p);
        if (po->p1030) {
            float t = (gXY - p->g6C) / (p->g70 - p->g6C);
            GClamp(&t, 0.0f, 1.0f);
            SetAsegaSpeed__FP5ASEGAf(po->p1030, (1.0f - t) * p->g74 + t * p->g78);
        }
    }
}
