struct CLOCK { float t; };
extern CLOCK g_clock __asm__("D_002DDED0");
struct CIRCLER { char pad[0x50]; float radsSpeed; };
float GModPositive(float, float);
/** @sly2-annot addr=0x1EE768
 * Purpose: not written up yet; only the symbol name (pre-existing / Sly 1-derived) describes it.
 * Confidence: mid (draft) - evidence: pre-existing symbol name only; behaviour not reviewed
 * See: docs/reference/functions/rendering.md#shdanim-0x1ed7a8-0x1ee988
 * @end-sly2-annot */
float UCompleteCircler(CIRCLER *pcircler)
{
    float angle = g_clock.t * pcircler->radsSpeed;
    return GModPositive(angle, 6.28318530718f) * 0.159154943092f;
}
