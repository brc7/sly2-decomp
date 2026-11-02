typedef int qword __attribute__((mode(TI)));
typedef union { qword q; float f; } U;
struct G;
extern struct G D_003DD490;
extern qword D_002A2DC0;
struct X { int a[4]; };
struct Y { qword v; char pad[0x18]; float f28; char pad2[0x14]; float f40; char pad3[0x1C]; };
struct X *STACKSCOPE_ctor(struct X *x);
void STACKSCOPE_ctor_1AE4A0(struct X *x, int n);
int func_0020FA50(void);
void IntersectSwBoundingBox(struct G *g, qword a, qword b, int (*fn)(void), int z, int *pa, int *pb);
int PsoHitTestLineObjects__FiP6VECTORT1iPP2SOP3LSG(int n, qword a, qword b, int c, int d, struct Y *y);
static __inline__ qword MAdd(qword p, qword up, float s) {
    U u;
    qword r;
    __asm__("qmtc2.ni %1, %0" : "=j"(u.q) : "r"(s));
    __asm__("vadda.xyzw $ACC, %1, $vf0\n\tvmaddx.xyzw %0, %2, %3x" : "=j"(r) : "j"(p), "j"(up), "j"(u.q));
    return r;
}
/** @sly2-annot addr=0x173610
 * Purpose: Cast a line along the up axis from pos+a*up to pos+b*up against SW objects (IntersectSwBoundingBox
 *   + PsoHitTestLineObjects, default filter FFilterSoNotSelfVt62); on a hit with a walkable normal (z>=0.7
 *   unless k) move *ppos to it
 * Info: proposed name FSnapToGround (T2); TU tu_173158
 * Notes: top50_round (0.55): Body (STACKSCOPE). Callers FStepplayerWillLand, FitStepToTerrain, StartBhvleap,
 *   SpawnCid119Guard, FGuardStuck
 * Confidence: low (draft) [T2 0.55] - evidence: semantic review; sources: r3b_rest+top50_round
 * See: docs/reference/functions/guards-ai.md#tu_173158-0x173158-0x174898,
 *   docs/reference/functions/guards-ai.md#step-0x203948-0x205e70,
 *   docs/reference/functions/player.md#stepplayer-0x13ff70-0x144638
 * @end-sly2-annot */
int func_00173610(qword *pv, float a, float b, int k, int (*fn)(void)) {
    struct X x;
    int na, nb;
    qword va, vb;
    STACKSCOPE_ctor(&x);
    va = MAdd(*pv, D_002A2DC0, a);
    vb = MAdd(*pv, D_002A2DC0, b);
    if (!fn) fn = func_0020FA50;
    {
    struct Y y;
    IntersectSwBoundingBox(&D_003DD490, va, vb, fn, 0, &na, &nb);
    if (!PsoHitTestLineObjects__FiP6VECTORT1iPP2SOP3LSG(1, va, vb, na, nb, &y) || y.f40 == 0.0f || (!k && y.f28 < 0.7f)) {
        STACKSCOPE_ctor_1AE4A0(&x, 2);
        return 0;
    }
    *pv = y.v;
    }
    STACKSCOPE_ctor_1AE4A0(&x, 2);
    return 1;
}
