struct S { int n0; int n4; int n8; };
struct L { int a; int b; int c; int d; };
void func_001EF0D8(struct L *pl);
/** @sly2-annot addr=0x1EECD0
 * Purpose: Pop every behaviour: if cpbhv>0 pop apbhv[0] (which pops everything stacked above it), then
 *   cpbhv=0
 * Info: proposed name ClearBhvstk (T1); TU bhvstk
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r6b_rest
 * See: docs/reference/functions/guards-ai.md#bhvstk-0x1eec60-0x1ef850,
 *   docs/reference/functions/player.md#stepplayer-0x13ff70-0x144638
 * @end-sly2-annot */
void func_001EECD0(struct S *p)
{
    struct L l;
    if (p->n4 > 0) {
        l.a = p->n8;
        func_001EF0D8(&l);
    }
    p->n4 = 0;
}
