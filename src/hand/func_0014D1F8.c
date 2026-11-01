struct X { char pad[8]; float g; float g2; char pad2[8]; float af[9]; };
/** @sly2-annot addr=0x14D1F8
 * Purpose: rDt = ? slot[0..8], rDtReal = slot[8]
 * Info: proposed name RecalcClockRate (T1); TU clock
 * Confidence: high [T1 0.85] - evidence: semantic review; sources: semantic_tu_docs/clock
 * See: docs/engine/main-loop-startup.md#2-clock-clockcpp-0x14cfe0-0x14d3d8,
 *   docs/engine/main-loop-startup.md#8-key-functions,
 *   docs/guides/live-pine.md#3-confirmed-addresses-2026-11-01
 * @end-sly2-annot */
void func_0014D1F8(struct X *p)
{
    unsigned int i;
    p->g = 1.0f;
    for (i = 0; i < 9; i++) {
        p->g *= p->af[i];
    }
    p->g2 = p->af[8];
}
