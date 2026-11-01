void func_001F7D90(void *, float);
void func_001F7D10(void *, float);
/** @sly2-annot addr=0x1F7BF8
 * Purpose: dur<=0: rate 0, alpha 0 (removes); else rate -1/dur
 * Info: proposed name FadeSoOut (T1); TU so
 * Notes: names_v2 v1 proposed CalculateSoAngularEffectWorld__FP2SOP6VECTORT1 (0.200)
 * Confidence: mid (draft) [T1 0.55] - evidence: static naming pass; sources: r6_1D5150
 * See: docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8,
 *   docs/reference/functions/player.md#jtgoal-0x144638-0x144b28
 * @end-sly2-annot */
void func_001F7BF8(void *p, float f)
{
    if (f <= 0.0f) {
        func_001F7D90(p, 0.0f);
        func_001F7D10(p, 0.0f);
    } else {
        func_001F7D90(p, -1.0f / f);
    }
}
