/* MATCH (Claude agent laneB22, 2026-10-23): from v8 (3). 2nd block: dt read as the scalar D_002DDED4 (g_clock.dt) through an "rV" asm input: expand keeps the MEM (V allows memory) but only r fits at reload, so reload loads it at the asm through the dying %hi reg (target lw v0,lo(v0) after the lqc2s); vsub then pin-free as "+j"(d = s.pos). */
/* v8 (Claude agent laneB21, 2026-10-23): aligned 7 -> 3. posT load: x8 ($v0) and a dummy z ($v1) kept live across it by two commented empty allocation-only asms, so the reload reg for the spilled posT is $a1 like the target (find_reg takes the first free reg in alloc order); the keep-alive asm has an output (+r po) so it is not volatile and reload_cse removes the a1 reload. Left: 2nd-block dt load (target: lui early + lw v0,lo(v0) right before the vsub = reload of a MEM). */
/* v7 (Claude agent laneB19, 2026-10-23): aligned 8 -> 7. Allocation-only "r"(p->x8) on the 2nd-block vsub/qmtc2 asm gives x8 $v1. Left: posT lq v0+por (target lq a1), lw dt scheduled before the lqc2s and dt in $a0 (target: reload-style lw v0,lo(v0) right before the vsub). */
/* v6 (Claude agent laneB15, 2026-10-23): aligned 9 -> 8. dv is a plain qword (spilled across the calls, so the dot reads its reload: lqc2 vf3) kept apart from d by a commented empty asm on d; posT a plain qword too (spill slots dv 0x40 / posT 0x50 in pseudo order). Left: posT load lq v0 + por a1 (target lq a1 directly), second block lw x8/dt regs. */
/* v5 (Claude agent laneB11, 2026-10-23): aligned 11 -> 9. One commented pin: d (second-block vsub output) to vf2. Left: first block dot VF numbers (dv reload vf3 / one vf2 swapped; output-order swap 16, =&j same), second block lw x8 -> v1 and lw dt scheduled before the lqc2s (target after). */
/* v4 (Claude agent laneB8, 2026-10-23): aligned 12 -> 11. Second block: vsub and the dt qmtc2 fused in ONE asm (vC0 as first input), plus a "memory" clobber on the vdiv asm (keeps the dt sqc2 before it, out of the b delay slot). Left: pos/vC0 VF regs swapped and lw dt scheduled early; first block unchanged. */
/* v3 (Claude agent laneB7, 2026-10-23): aligned 13 -> 12. PpoClosestSw takes a 2nd qword arg (posT; it sq a1 to its stack); posT as a U memory local so the load goes straight into a1. */
/* v2 (Claude agent, 2026-10-23): aligned 15 -> 13: qmtc2 asm before the vsub, vsub as "=j"(d) : s.pos, vC0 (gets dt vf3). Left: target schedules vsub before qmtc2 with s.pos vf2/vC0 vf1, output vf2. */
// WIP (Claude agent): 15 diffs - VU reg numbering only: dv reload should be vf3 / one-vector vf2 (ours swapped), second block wants lw of g_clock.dt after the lqc2s and vsub in vf2; posT lq via $a1.
typedef int qword __attribute__((mode(TI)));
typedef union { qword q; float f[4]; } U;
struct VTE { short d; short pad; void (*pfn)(); };
struct POLY { float a, b, c; };
struct O { int pad[6]; struct VTE *pvt; };
struct X { char pad[0x70]; qword v70; char pad80[0x90 - 0x80]; struct P *p90; short pad94[3]; short s9A; int pad9C; int xA0;
    char padA4[0xC0 - 0xA4]; qword vC0; qword vD0; };
struct P {
    int x0; void *x4; struct X *x8; int xC; int pad10[4]; qword v20; struct POLY poly30; int pad3C; struct POLY poly40; int pad4C;
    float g50; float g54; float g58; float g5C; float g60; int x64; int x68; int x6C; void *x70; int x74;
};
extern char D_003DD490[];
extern qword D_002A2DD0;
extern float D_002A45A8;
struct CLOCK { float t; int dt; };
extern struct CLOCK D_002DDED0;
extern int D_002DDED4;  /* g_clock.dt */
void *PloFindSwObject__FP2SWi3OIDP2LO(void *pg, int k, int n, void *pv);
void *PpoClosestSw(void *pg, qword pos);
qword func_001A62E8(void *po);
void func_002301D0(void *a, struct X *b);
qword ConvertXfVec(int n, int k, qword v);
void CalculateVectorPanTilt(qword q, float *pa, float *pb);
static __inline__ float GEval(struct POLY *pp, float t) { return pp->a + t * (pp->b + t * pp->c); }
static __inline__ float GClamp(float g, float lo, float hi) { float r; if (g < lo) r = lo; else if (hi < g) r = hi; else r = g; return r; }
static __inline__ float GLength(U *pv, qword a)
{
    qword t;
    float g;
    __asm__("vmul.xyz %0, %0, %2\n\tvaddw.x %1, $vf0, $vf0w\n\tvadday.x ACC, %0, %0y\n\tvmaddz.x %0, %1, %0z" : "+j"(a), "=j"(t) : "j"(pv->q));
    __asm__("vsqrt Q, %0x\n\tvwaitq\n\tvaddq.x %0, $vf0, Q" : "+j"(a));
    __asm__("qmfc2.ni %0, %1" : "=r"(g) : "j"(a));
    return g;
}

void UpdateActLookAt(struct P *p)
{
    struct { qword pos; qword vel; } s;
    U dt;
    float ag[2];
    qword dv;
    qword posT;
    void *po;
    float g;
    int k;
    qword vel;

    if (p->x70 == 0 && p->x6C != -1)
        p->x70 = PloFindSwObject__FP2SWi3OIDP2LO(D_003DD490, 0x104, p->x6C, p->x4);
    {
        struct X *x8 = p->x8;
        int z;
        __asm__("" : "=r"(z));  /* empty asm, allocation only: z occupies $v1 across the posT load */
        posT = x8->v70;
        po = p->x70;
        __asm__("" : "+r"(po) : "r"(x8), "r"(z));  /* empty asm, allocation only: x8 ($v0) and z ($v1) stay live past the posT load, so reload picks $a1 for it (non-volatile: has an output, so reload_cse can drop the later a1 reload) */
    }
    if (!po)
        po = PpoClosestSw(D_003DD490, posT);
    if (po) {
        qword d, t;
        d = func_001A62E8(po);
        __asm__("vsub.xyzw %0, %0, %1" : "+j"(d) : "j"(posT));
        dv = d;
        __asm__("" : "+j"(d));  /* empty asm: keeps d and dv separate pseudos */
        __asm__("vmul.xyz %0, %0, %2\n\tvaddw.x %1, $vf0, $vf0w\n\tvadday.x ACC, %0, %0y\n\tvmaddz.x %0, %1, %0z" : "+j"(d), "=j"(t) : "j"(dv));
        __asm__("vsqrt Q, %0x\n\tvwaitq\n\tvaddq.x %0, $vf0, Q" : "+j"(d));
        __asm__("qmfc2.ni %0, %1" : "=r"(g) : "j"(d));
    } else {
        g = D_002A45A8;
    }
    k = (p->g60 < g) ? p->x68 : p->x64;
    if (k != p->xC) {
        struct P *p90;
        p->xC = k;
        func_002301D0(p->x4, p->x8);
        p90 = p->x8->p90;
        if (p90 != p) {
            struct O *po2 = (struct O *)p90;
            ((void (*)(struct O *))po2->pvt[4].pfn)((struct O *)((char *)po2 + po2->pvt[4].d));
            return;
        }
    }
    if (p->g60 < g) {
        s.pos = p->v20;
        s.vel = D_002A2DD0;
        p->x74 = 0;
    } else {
        CalculateVectorPanTilt(ConvertXfVec(0, p->x8->xA0, dv), &ag[0], &ag[1]);
        ag[0] = GClamp(GEval(&p->poly30, ag[0]), p->g50, p->g54);
        ag[1] = GClamp(GEval(&p->poly40, ag[1]), p->g58, p->g5C);
        ((float *)&s.pos)[2] = ag[0];
        ((float *)&s.pos)[1] = -ag[1];
        ((int *)&s.pos)[0] = 0;
        if (p->x74) {
            qword d = s.pos;
            __asm__("vsub.xyzw %0, %0, %2\n\tqmtc2.ni %3, %1" : "+j"(d), "=j"(dt.q) : "j"(p->x8->vC0), "rV"(D_002DDED4));  /* "rV": MEM kept to reload, which loads it into the dying %hi reg */
            __asm__("vdiv Q, $vf0w, %1x\n\tvwaitq\n\tvmulq.xyzw %0, %2, Q" : "=j"(vel) : "j"(dt.q), "j"(d) : "memory");
        } else {
            vel = D_002A2DD0;
        }
        s.vel = vel;
        p->x74 = 1;
    }
    p->x8->vC0 = s.pos;
    p->x8->vD0 = s.vel;
    p->x8->s9A = 3;
}
