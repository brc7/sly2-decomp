// MATCH (Claude agent, laneB6, 2026-10-23), from the 116/aligned-25 draft: (1) no f4/f2 locals (`(grf >> 2) & 1` and `grf & 2` at their uses), so the vtable address is hoisted into fp; (2) AddXfAct takes pn as a 3rd argument; (3) pn->xC before pn->x14; (4) floats before pp in the prototype (prologue copy order).
struct VTE { short d; short pad; void (*pfn)(); };
struct POLY { float a, b, c; };
struct E60 { unsigned int k; int x4; };
struct E68 { int x0; int x4; int x8; };
struct EV { int a[4]; };
struct DL { void *pvFirst; void *pvLast; int ibDle; };
struct T {
    int x0; char pad4[0x50 - 4]; struct DL dl50; int x5C; struct E60 *ae60; int c64; struct E68 *ae68; int x6C;
    int x70; int pad74; float g78; int pad7C; struct POLY poly; int pad8C; float g90; float g94; int cev; struct EV *aev; int xA0;
};
struct N34 { int x0; int x4; int x8; int xC; int x14_pad; int x14; VTE *pvt; int x1C; int x20; struct P *p24; struct N34 *pnNext; };
struct N38 { int x0; int x4; int x8; int xC; struct N38 *pnNext; };
struct P {
    int x0; int x4; int x8; VTE *pvt; int x10; struct T *pt; int x18; float g1C; float g20; char pad24[0x34 - 0x24];
    struct N34 *pn34; struct N38 *pn38; int x3C; float g40; int x44; float g48; float g4C; int x50; int pad54[2]; int x5C;
};
struct G { P *p; G(P *pp) : p(pp) { ((void (*)(P *))p->pvt[1].pfn)((P *)((char *)p + p->pvt[1].d)); }
    ~G() { ((void (*)(P *))p->pvt[2].pfn)((P *)((char *)p + p->pvt[2].d)); } };
extern "C" {
extern char D_003E1224[];
extern char D_003E1218[];
extern VTE D_002CAF48[];
extern VTE D_002CAF08[];
extern char D_002DE578[];
P *PvAllocSlotheapClear(void *);
void *func_002333B4(void *, int, int);
float GGetDifficulty(void *);
void func_00131A00(P *, float);
void AppendDlEntry__FP2DLPv(void *, void *);
void func_00256EE0(P *, int);
void func_0012FE58(struct T *, int, struct E60 *, int *, int *);
void func_001F8668(int, long);
void *func_0012E1F0(unsigned int);
void AddXfAct(int, int, void *);
void func_00130010(struct T *, int, struct E68 *, int *, int *);
void *func_0012CE50(unsigned int);
void SetAsegaHandsOff(P *, int);
void HandleAsegaEvent(P *, struct EV *);
void func_00230CF0(P *, int);
void HandleAsegaEventsFF(P *);
void func_00130EB0(P *);
}
static inline float GClamp(float g, float lo, float hi) { float r; if (g < lo) r = lo; else if (hi < g) r = hi; else r = g; return r; }
static inline float GEval(struct POLY *pp, float t) { return pp->a + t * (pp->b + t * pp->c); }

/** @sly2-annot addr=0x12F9B8
 * Purpose: Core of applying an animation segment to an ALO hierarchy: allocates an ASEGA from the pool, links
 *   it, binds channels, sets speed, fast-forwards events, UpdateAsegaIeaCur
 * Info: proposed name ApplyAseg (T1); TU aseg
 * Notes: verifier: 0x12F9B8 is the ApplyAseg body (Sly 2 dropped the OVR arg); r1 ApplyAsegOvr superseded
 *   (was ApplyAsegOvr)
 * Confidence: high [T1 0.80] - evidence: semantic review; sources: verify_r1r2_summary
 * See: docs/engine/animation.md#1-aseg-and-asega-asegcpp-0x12f248-asegacpp-0x1308a8,
 *   docs/reference/functions/animation.md#aseg-0x12f248-0x1308a8,
 *   docs/reference/functions/animation.md#sm-0x1efa90-0x1f1768
 * @end-sly2-annot */
extern "C" void func_0012F9B8(struct T *pt, int x, int grf, float g0, float g1, P **pp)
{
    P *p = PvAllocSlotheapClear(D_003E1224);
    int i, j, k;
    func_002333B4(p, 0, 0x60);
    p->x0 = 0;
    p->x4 = 0;
    p->pvt = D_002CAF48;
    p->x5C = 0;
    p->pt = pt;
    p->x18 = x;
    p->g1C = g0;
    p->x3C = grf;
    p->g48 = pt->g78;
    {
        p->g4C = GClamp(GEval(&pt->poly, GGetDifficulty(D_002DE578)), pt->g90, pt->g94);
    }
    func_00131A00(p, g1);
    AppendDlEntry__FP2DLPv(D_003E1218, p);
    AppendDlEntry__FP2DLPv(&pt->dl50, p);
    ((void (*)(P *))p->pvt[1].pfn)((P *)((char *)p + p->pvt[1].d));
    G g(p);
    func_00256EE0(p, pt->x0);
    if (pp)
        *pp = p;
    for (i = 0; i < pt->x5C; i++) {
        struct E60 *pe = &pt->ae60[i];
        if (!(pe->k & 0x40000000)) {
            int a, b;
            func_0012FE58(pt, x, pe, &a, &b);
            if (b) {
                func_001F8668(a, 0);
                struct N34 *pn = (struct N34 *)func_0012E1F0(0x2C);
                pn->pvt = D_002CAF08;
                pn->x0 = 0;
                if (pe->k & 0x80000000)
                    pn->x20 = pe->x4;
                else
                    pn->x1C = pe->x4;
                pn->x4 = a;
                pn->x8 = b;
                pn->xC = pt->x6C;
                pn->x14 = (grf & 1) ? 7 : 5;
                pn->p24 = p;
                pn->pnNext = p->pn34;
                p->pn34 = pn;
                AddXfAct(a, b, pn);
            }
        }
    }
    for (j = 0; j < pt->c64; j++) {
        struct E68 *pe = &pt->ae68[j];
        int a, b;
        func_00130010(pt, x, pe, &a, &b);
        if (b) {
            struct N38 *pn = (struct N38 *)func_0012CE50(0x14);
            pn->x0 = pe->x8;
            pn->x4 = b;
            pn->x8 = a;
            pn->xC = pe->x0;
            pn->pnNext = p->pn38;
            p->pn38 = pn;
            P *po = (P *)a;
            ((void (*)(P *, int))po->pvt[44].pfn)((P *)((char *)po + po->pvt[44].d), pe->x0);
        }
    }
    SetAsegaHandsOff(p, pt->x70);
    p->g20 = g0;
    for (k = 0; k < pt->cev; k++)
        HandleAsegaEvent(p, &pt->aev[k]);
    p->g1C = p->g20;
    func_00230CF0(p, (grf >> 2) & 1);
    if (grf & 2) {
        if (p->g40 < 0.0f)
            p->x50 = pt->xA0;
        HandleAsegaEventsFF(p);
    } else {
        func_00130EB0(p);
    }
    ((void (*)(P *, int, int, int, int, int))p->pvt[3].pfn)((P *)((char *)p + p->pvt[3].d), 2, 0, 0, 0, 0);
}
