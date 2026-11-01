// MATCH (Claude agent, laneC43 2026-10-24): v7 + case 5 `pvt = p->pvt` read after the qmtc2 asm, which gets a "memory" clobber (lw then can't fill the mfc1->qmtc2 slot). No register pins.
// v7 (Claude agent, laneC42 2026-10-24): aligned 12 -> 1 with NO register pins. Dist3 ends with the guide's mtc1-from-qword helper `__asm__("mtc1 %1, %0" : "=f"(g) : "r"(d), "f"(k))` (gcc emits the qmfc2 v1 itself, asm cost 1 so max.s follows the mtc1 like the target); the $3 pin and the qmfc2 asm are gone. Left (1): case 5 lw pvt fills the slot between the reload mfc1 and the qmtc2 asm. A "$3" clobber on the qmtc2 asm fixes the order but pvt then takes a2 (with the vmulx "$2") or v0 (without it); pins on pvt/pfn on top: 3-11. The game's mfc1 VU_FLOAT ctor asm: 16-22.
// v6 (Claude agent, laneC33 2026-10-24): aligned 1 -> 12 (WORSE, policy version). The case-3 mtc1 asm is replaced by a pointer pun `return *(float *)&t;` (gcc emits the same mtc1 v1,f1), "f"(k) moved to the qmfc2 asm as the allocation-only operand. gcc's own mtc1 (movsf) has scheduler cost 2, so the lw p->pe again fills the max.s hazard slot, and the p1C lw gets a0 (target v0). union{int;float} 14, float-typed $3 qmfc2 output 14, no "f"(k) 17, "f"(k) on a separate empty asm 26.
// v5 (Claude agent, laneC32 2026-10-24): aligned 11 -> 1. (1) Case 3: its own Dist3/LINKE3 with qmfc2 into an int (ONE register pin, $3 like the target) plus a separate mtc1 asm, which has latency 1 in the scheduler (the reload movsf had 2), so max.s issues right after the mtc1 and the lw p->pe follows it (target keeps the mtc1 hazard nop). An allocation-only "f"(k) (k = the 250.0f GMax limit) on the mtc1 gives 250 f0 / distance f1 like the target. (2) Case 5: `qword vv = *pv` before the qmtc2 (lqc2 before mfc1, mfc1 reuses v0), asm outputs into plain qwords sq/vq copied into the unions afterwards, a pvt local passed as an allocation-only "r"(pvt) to the vmulx (sched1 otherwise sinks the vtable lw below the union stores) and a clobber of the dead v0 on the vmulx (pvt v1, pfn v0). Left: lw pvt fills the mfc1 -> qmtc2 stall (target: qmtc2 right after the mfc1, same reload-latency effect as case 3).
// v4 (Claude agent, laneC6 2026-10-23): 13 -> 11: case 3 writes `p->pe = func_001CE888(...); if (p->pe) LINKE(p->pe, pot->pr, p)` (no pe local). Left: case 3 mtc1->max.s hazard nop (our lw p->pe fills it).
// v3 (Claude agent, laneC5 2026-10-23): aligned 46 -> 13: case-4 `pot4 = p->pot` read (gives the s0 copy), Dist sqrt asm split from its qmfc2, LINKE macro as a plain block (do/while(0) added a nop). Left: case 3 LINKE scheduling.
// ~235/598 diffs, mostly offsets after a few small sched diffs (Dist/vsqrt scheduling, GMax regs, loop nops); frame 0x110 and layout match.
typedef int qword __attribute__((mode(TI)));
struct VTE { short d; short pad; void (*pfn)(); };
struct VT { struct VTE a[64]; };
struct CLOCK { float t; float dt; };
struct GS { char pad[0x4638]; void *p4638; };
struct ST { char pad[0xC]; int xC; char pad1[0x18 - 0x10]; int x18; int c1C; int a20[1]; };
struct SA0 { int x0; int x4; float g8; };
struct E {
    char pad[0x18]; float g18; void *p1C; char pad1[0xA0 - 0x20]; struct SA0 sA0;
    char pad2[0xEC - 0xAC]; struct VT *pvt; char pad3[0x104 - 0xF0]; void *p104;
};
struct O;
struct Q { char pad[0xC]; struct VT *pvt; int x10; char pad1[0x18C - 0x14]; float g18C; struct O *p190; struct O *p194; };
struct R { char pad[0xC]; struct VT *pvt; };
struct OT { char pad[0x11A4]; struct R *pr; struct O *p11A8; };
struct O {
    char pad[0xC]; struct VT *pvt; char pad1[0xE0 - 0x10]; int cE0; char *aE4;
    char pad2[0x158 - 0xE8]; struct ST *pst; struct Q *pq; struct OT *pot; struct R *p164;
    char pad3[0x180 - 0x168]; float g180; int st; float g188; struct E *pe;
};
struct MSG { int k; void *p; };
extern struct CLOCK D_002DDED0;
extern struct GS D_003DD490;
extern char D_002DEB30[];
extern qword D_002A2DD0;
extern qword D_002A4B60;
void func_001CB4B8(void *pv, struct O *p);
void func_001CB488(void *pv, struct O *p);
void func_001F6A68(struct O *p);
void func_001F6978(struct O *p, int n);
void func_001F2AD0(struct O *p, void *pv);
void func_001F6AC8(struct O *p, int n);
void func_001F6B48(struct O *p, int n);
void func_001618F8(struct GS *pgs, float g);
qword func_001A62E8(void *p);
void func_00156C80(struct GS *pgs, int n, qword v, struct Q *pq, void *pv);
void func_001619A0(void *pv, int n, struct O *p);
void func_00161958(struct GS *pgs);
void func_001F5720(struct O *p, int n);
void func_001F57C0(struct O *p, int n);
void func_001F5880(struct O *p, qword v);
void func_001A58D0(struct O *p, void *pv, int n);
void func_001A6BE8(struct O *p, void *pv);
void func_001F7C68(struct O *p, float g);
struct E *func_001CE888(struct GS *pgs, int n, int m);
void func_001CF578(struct E *pe, struct O *p);
void func_001CFD30(struct E *pe);
float GRandInRange__Fff(float a, float b);
void func_0013C560(struct O *p, int st);

#define VCALL(po, slot, T) ((T)(po)->pvt->a[slot].pfn)((char *)(po) + (po)->pvt->a[slot].d
typedef void (*PFN3)(void *, int, int, int, int, int);
typedef void (*PFNV)(void *);
typedef void (*PFNQ)(void *, qword);
typedef void *(*PFNP)(void *);
typedef void (*PFNM)(void *, struct MSG *);
typedef void (*PFNE)(void *, qword, int, float);

static inline float Dist(qword a, qword b)
{
    qword d;
    float g;
    __asm__("vsub.xyzw %0, %1, %2" : "=j"(d) : "j"(a), "j"(b));
    __asm__("vmul.xyz %0, %0, %0\n\tvaddw.x $vf2, $vf0, $vf0w\n\tvadday.x ACC, %0, %0y\n\tvmaddz.x %0, $vf2, %0z"
        : "+j"(d) : : "vf2");
    __asm__("vsqrt Q, %0x\n\tvwaitq\n\tvaddq.x %0, $vf0, Q" : "+j"(d));
    __asm__("qmfc2.ni %0, %1" : "=r"(g) : "j"(d));
    return g;
}

static inline float Dist3(qword a, qword b, float k)
{
    qword d;
    float g;
    __asm__("vsub.xyzw %0, %1, %2" : "=j"(d) : "j"(a), "j"(b));
    __asm__("vmul.xyz %0, %0, %0\n\tvaddw.x $vf2, $vf0, $vf0w\n\tvadday.x ACC, %0, %0y\n\tvmaddz.x %0, $vf2, %0z"
        : "+j"(d) : : "vf2");
    __asm__("vsqrt Q, %0x\n\tvwaitq\n\tvaddq.x %0, $vf0, Q" : "+j"(d));
    __asm__("mtc1 %1, %0" : "=f"(g) : "r"(d), "f"(k)); /* mtc1 from a qword (gcc emits the qmfc2); "f"(k): allocation-only operand (Claude agent) */
    return g;
}

static inline float GMax(float a, float b) { return a < b ? b : a; }

#define LINKE(PE, PA, PB) { \
    struct VTE *pvte = &(PE)->pvt->a[1]; \
    float g; \
    ((PFNE)pvte->pfn)((char *)(PE) + pvte->d, func_001A62E8(PA), 0, 1.0f); \
    func_001CF578(p->pe, p); \
    g = Dist(func_001A62E8(PA), func_001A62E8(PB)); \
    p->pe->p1C = (PB); \
    p->pe->g18 = GMax(250.0f, g) * 0.002f; \
    { struct SA0 *ps = &p->pe->sA0; ps->x0 = 0; ps->g8 = -3000.0f; ps->x4 = 0; } \
    p->pe->p104 = (PA); \
    func_001CFD30(p->pe); \
    VCALL(p, 8, PFNV)); \
}
#define LINKE3(PE, PA, PB) { \
    struct VTE *pvte = &(PE)->pvt->a[1]; \
    float g; \
    ((PFNE)pvte->pfn)((char *)(PE) + pvte->d, func_001A62E8(PA), 0, 1.0f); \
    func_001CF578(p->pe, p); \
    g = Dist3(func_001A62E8(PA), func_001A62E8(PB), 250.0f); \
    p->pe->p1C = (PB); \
    p->pe->g18 = GMax(250.0f, g) * 0.002f; \
    { struct SA0 *ps = &p->pe->sA0; ps->x0 = 0; ps->g8 = -3000.0f; ps->x4 = 0; } \
    p->pe->p104 = (PA); \
    func_001CFD30(p->pe); \
    VCALL(p, 8, PFNV)); \
}

static inline void Notify(struct R *pr, int k)
{
    struct MSG msg;
    msg.p = pr;
    msg.k = k;
    VCALL(pr, 51, PFNM), &msg);
}

void func_0013C560(struct O *p, int st)
{
    int stOld;
    struct OT *pot;
    int i;
    struct OT *pot4;

    if (p->st == st)
        return;
    if (D_003DD490.p4638 != 0) {
        int fOld = p->st >= 2;
        int fNew = st >= 2;
        if (fOld) {
            if (!fNew)
                func_001CB4B8(D_003DD490.p4638, p);
        } else if (fNew) {
            func_001CB488(D_003DD490.p4638, p);
        }
    }
    stOld = p->st;
    pot = p->pot;
    switch (stOld) {
    case -1:
        break;
    case 0:
        func_001F6A68(p);
        for (i = 0; i < p->pst->c1C; i++)
            func_001F6978(p, p->pst->a20[i]);
        for (i = 0; i < p->cE0; i++)
            func_001F2AD0(p, p->aE4 + i * 0xB0);
        if (p->pst->x18 != -1)
            func_001F6AC8(p, p->pst->x18);
        break;
    case 1:
        if (p->pst->xC)
            func_001618F8(&D_003DD490, p->pq->g18C + 0.99f);
        func_00156C80(&D_003DD490, 1, func_001A62E8(p->pq), p->pq, (char *)p->pq + 0x198);
        if (st == 2)
            func_001619A0(D_002DEB30, 0x2F, p);
        p->pq->p194 = 0;
        p->pq = 0;
        VCALL(p, 3, PFN3), 5, 0, 0, 0, 0);
        break;
    case 2:
        p->pe = 0;
        if (st == 4)
            p->g180 = 0.3f;
        break;
    case 3:
        p->pe = 0;
        break;
    case 4:
        pot4 = p->pot;
        p->pot = 0;
        VCALL(pot4->pr, 9, PFNV));
        pot4->p11A8 = 0;
        if (p->pst->x18 != -1)
            func_001F6AC8(p, p->pst->x18);
        if (p->pst->xC)
            func_00161958(&D_003DD490);
        break;
    case 5: {
        struct R *pr = p->p164;
        if (pr)
            Notify(pr, 1);
        func_001F5720(p, 1);
        func_001F57C0(p, 1);
        VCALL(p, 35, PFNQ), D_002A2DD0);
        VCALL(p, 36, PFNQ), D_002A2DD0);
        func_001F5880(p, D_002A2DD0);
        break;
    }
    case 6:
        p->g180 = 0.0f;
        break;
    }

    p->st = st;
    p->g188 = D_002DDED0.t;

    switch (st) {
    case 0:
        p->pst = 0;
        func_001A58D0(p, 0, 0);
        VCALL(p, 9, PFNV));
        break;
    case 1:
        func_00156C80(&D_003DD490, -1, D_002A2DD0, 0, (char *)p->pq + 0x198);
        func_001A6BE8(p, VCALL(p->pq, 31, PFNP)));
        func_001A58D0(p, (void *)p->pq->x10, 3);
        p->pq->p194 = p;
        if (stOld != 3) {
            VCALL(p, 9, PFNV));
            func_001F7C68(p, 0.5f);
        }
        VCALL(p, 3, PFN3), 6, 0, 0, 0, 0);
        break;
    case 2:
    case 6:
        VCALL(p->pot->pr, 8, PFNV));
        if (p->st == 2) {
            p->pe = func_001CE888(&D_003DD490, 6, 0);
        } else {
            p->pe = 0;
            VCALL(p, 3, PFN3), 6, 0, 0, 0, 0);
            VCALL(p, 3, PFN3), 5, 0, 0, 0, 0);
        }
        {
            struct E *pe = p->pe;
            if (pe)
                LINKE(pe, p, p->pot->pr);
        }
        func_001A6BE8(p, VCALL(p->pot->pr, 31, PFNP)));
        func_001A58D0(p, p->pot->pr, 3);
        if (!p->pe)
            func_0013C560(p, 4);
        break;
    case 3: {
        struct E *pe;
        func_001A6BE8(p, VCALL(p->pq, 31, PFNP)));
        func_001A58D0(p, (void *)p->pq->x10, 3);
        p->pe = func_001CE888(&D_003DD490, 6, 0);
        if (p->pe)
            LINKE3(p->pe, pot->pr, p);
        if (!p->pe)
            func_0013C560(p, 1);
        break;
    }
    case 4:
        p->pot->p11A8 = p;
        if (p->pst->x18 != -1)
            func_001F6B48(p, p->pst->x18);
        break;
    case 5: {
        union { qword q; float f[4]; } v;
        union { qword q; float f[4]; } u;
        union { qword q; float f[4]; } s;
        union { qword q; float f[4]; } w;
        float a, b, c;
        float lo = -6.2831855f, hi = 6.2831855f;
        (void)&w;
        func_001A58D0(p, 0, 3);
        func_001F5720(p, 0);
        func_001F57C0(p, 0);
        {
            qword *pv = (qword *)VCALL(p, 31, PFNP));
            qword vv = *pv;
            struct VT *pvt;
            qword sq, vq;
            __asm__("qmtc2.ni %1, %0" : "=j"(sq) : "r"(1000.0f) : "memory"); /* "memory" clobber (Claude agent): with pvt read after this asm, keeps the vtable lw after the qmtc2 like the target */
            pvt = p->pvt;
            __asm__("vmulx.xyzw %0, %1, %2x" : "=j"(vq) : "j"(vv), "j"(sq), "r"(pvt) : "$2"); /* "r"(pvt): allocation-only (Claude agent), keeps the vtable load before the vmulx like the target; "$2": clobber of the dead v0 (Claude agent), so pvt takes v1 like the target */
            s.q = sq;
            v.q = vq;
            ((PFNQ)pvt->a[35].pfn)((char *)p + pvt->a[35].d, v.q);
        }
        a = GRandInRange__Fff(lo, hi);
        b = GRandInRange__Fff(lo, hi);
        c = GRandInRange__Fff(lo, hi);
        u.f[0] = a;
        u.f[1] = b;
        u.f[2] = c;
        VCALL(p, 36, PFNQ), u.q);
        func_001F5880(p, D_002A4B60);
        func_001619A0(D_002DEB30, 0x30, p);
        break;
    }
    }
}
