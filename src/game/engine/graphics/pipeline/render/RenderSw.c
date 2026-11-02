struct VTE { short d; short pad; void (*pf)(); };
struct LO { char pad[0xC]; struct VTE *vt; };
struct E { int grf; struct LO *plo; };
struct G { char pad[0x3D5C]; int x3D5C; int x3D60; int x3D64; char p1[0x3F14 - 0x3D68]; int c; int x3F18; int *pi; struct E *ae; };
extern char D_003B3980[];
extern void *D_003D4A50;
extern char D_003DA0F0[];
extern char D_002DEA00[];
extern char D_002DEA28[];
void PrepareSwLights(struct G *p);
void SubmitSwRelightAll(struct G *p);
void func_001DAC58(void *p);
void func_001574A0(void *p);
/** @sly2-annot addr=0x1CDA80
 * Purpose: SW vt8: per-frame world render entry (called from main loop)
 * Info: proposed name RenderSw (T1); class SW slot8; TU render
 * Notes: runtime: bumps frame counter SW+0x3D60 (== g_cframe delta)
 * Confidence: high [T1 0.85] - evidence: runtime (save-state RAM); sources: r5_1AE130+runtime
 * See: docs/engine/overview.md#2-the-frame-loop-exact-order,
 *   docs/engine/overview.md#5-per-frame-update-and-render, docs/engine/rendering.md#1-frame-structure
 * @end-sly2-annot */
void RenderSw(struct G *p)
{
    int i;
    PrepareSwLights(p);
    p->x3D60++;
    D_003D4A50 = D_003B3980;
    if (p->x3D5C != p->x3D64) {
        p->x3D64 = p->x3D5C;
        SubmitSwRelightAll(p);
    }
    p->pi = &i;
    for (i = 0; i < p->c; i++) {
        struct E *pe = &p->ae[i];
        if ((pe->grf & 0x88) == 8)
            ((void (*)(void *))pe->plo->vt[19].pf)((char *)pe->plo + pe->plo->vt[19].d);
    }
    p->pi = 0;
    func_001DAC58(D_003DA0F0);
    func_001574A0(D_002DEA00);
    func_001574A0(D_002DEA28);
}
