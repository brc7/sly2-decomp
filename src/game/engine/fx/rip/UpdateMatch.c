struct VTE { short d; short pad; void (*pfn)(); };
struct O { int a, b, c; struct VTE *vt; };
extern struct CLOCK { float t; } D_002DDED0;
struct S { char p0[0x14]; float g14; float g18; struct O *po; char p1[0x104-0x20]; int f104; };
void LaunchMatch__FP5MATCH(struct S *p);
void func_001CE980(struct S *p);
/** @sly2-annot addr=0x1D0038
 * Purpose: MATCH Update: LaunchMatch / RemoveRip
 * Info: proposed name UpdateMatch (T1); TU rip
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r5_1AE130+verdicts_A2+verify_side
 * See: docs/engine/particles-effects.md#6-key-functions,
 *   docs/reference/functions/particles-effects.md#rip-0x1cdcd0-0x1d1370
 * @end-sly2-annot */
void UpdateMatch(struct S *p) {
    if (D_002DDED0.t >= p->g14 && p->f104) LaunchMatch__FP5MATCH(p);
    if (D_002DDED0.t - p->g14 > p->g18) {
        struct O *po = p->po;
        struct VTE *e = &po->vt[8];
        ((void (*)(void *))e->pfn)((char *)po + e->d);
        func_001CE980(p);
    }
}
