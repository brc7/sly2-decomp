/* near-miss: target materialises 0xFFFFFFFF mask as addiu -1; dsrl32 (assembler dli expansion?), ours lui 0xFFFF; dsrl32 */
struct T { char pad[0x50]; unsigned long last; int hi; };
/** @sly2-annot addr=0x14D240
 * Purpose: 64-bit tick: CP0 Count masked to 32 bits; if less than last, wrap counter++ (state at +0x50/+0x58)
 * Info: proposed name TickNow (T1); TU clock
 * Confidence: high [T1 0.85] - evidence: semantic review; sources: semantic_tu_docs/clock
 * See: docs/engine/main-loop-startup.md#8-key-functions,
 *   docs/reference/functions/main-loop-startup.md#clock-0x14cfe0-0x14d3d8
 * @end-sly2-annot */
unsigned long func_0014D240(struct T *p)
{
    unsigned long c;
    __asm__ volatile ("mfc0 %0, $9" : "=r"(c));
    c &= 0xFFFFFFFF;
    if (c < p->last) p->hi++;
    p->last = c;
    return ((unsigned long)p->hi << 32) | c;
}
