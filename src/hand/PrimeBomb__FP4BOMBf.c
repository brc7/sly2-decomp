struct S { char pad[0x154]; float f154; };
extern float D_002DDED0;
/** @sly2-annot addr=0x13B5C8
 * Purpose: tDetonate = min(tDetonate, now + dt)
 * Info: proposed name PrimeBomb (T1); TU bomb
 * Notes: runtime5a #12: dart tDetonate = fire + 4.0
 * Confidence: high [T1 0.90] - evidence: runtime (save-state RAM); sources:
 *   r2b_rest+verdicts_followups+runtime
 * See: docs/engine/gameplay-objects.md#key-functions,
 *   docs/reference/functions/gameplay-objects.md#bomb-0x13a9e8-0x13bdf0
 * @end-sly2-annot */
void PrimeBomb__FP4BOMBf(struct S *p, float f) { p->f154 = (p->f154 < D_002DDED0 + f) ? p->f154 : D_002DDED0 + f; }
