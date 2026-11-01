struct VTE { short d; short p; void (*pfn)(void *); };
struct VT { struct VTE a[9]; };
struct P { char pad[0xC]; struct VT *pvt; char pad2[0x40 - 0x10]; int v40; char pad3[0x130 - 0x44]; float g; };
/** @sly2-annot addr=0x1F7D90
 * Purpose: If rate != 0: Add (vt8) and set grf bit 2; store +0x130
 * Info: proposed name SetSoFadeRate (T1); TU so
 * Notes: names_v2 v1 proposed RotateSoToMat__FP2SOP7MATRIX3 (0.026)
 * Confidence: high [T1 0.85] - evidence: static naming pass; sources: r6_1D5150+verdicts_tu_r4r6
 * See: docs/engine/physics-collision.md#7-key-functions,
 *   docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8
 * @end-sly2-annot */
void func_001F7D90(struct P *p, float g)
{
    if (g != 0.0f) {
        int v;
        p->pvt->a[8].pfn((char *)p + p->pvt->a[8].d);
        v = p->v40;
        if ((v ^ 1) & 1) *(int *)v |= 2;
        else p->v40 = v | 2;
    }
    p->g = g;
}
