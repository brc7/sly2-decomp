void func_0012F9B8(int, int, int, int *, float, float);
/** @sly2-annot addr=0x130488
 * Purpose: ApplyAsegOvr without override
 * Info: proposed name PasegaApplyAseg (T1); TU aseg
 * Notes: verifier WRONG: ApplyAseg -> PasegaApplyAseg | verifier: purpose partly wrong: Calls 0x12F9B8(paseg,
 *   palo, grfapl, &pasega, t, svt), which is itself the ApplyAseg body (no OVR arg), and returns pasega. That
 *   is Sly 1 PasegaApplyAseg; it sit | names_v2 v1 proposed ApplyAseg__FP4ASEGP3ALOffiPP5ASEGA (0.000) |
 *   calls ApplyAseg body 0x12F9B8 and returns pasega (verifier WRONG->better)
 * Confidence: high [T1 0.80] - evidence: Sly 1 / Lilly reference; static naming pass; sources:
 *   verify_r1r2+verdicts_tu_r1r2
 * See: docs/engine/animation.md#1-aseg-and-asega-asegcpp-0x12f248-asegacpp-0x1308a8,
 *   docs/engine/animation.md#7-key-functions, docs/reference/functions/animation.md#aseg-0x12f248-0x1308a8
 * @end-sly2-annot */
int func_00130488(int a, int b, int c, float f, float g) { int r; func_0012F9B8(a, b, c, &r, f, g); return r; }
