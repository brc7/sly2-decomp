struct S { char pad[0xAC]; int a; int b; int c; };
/** @sly2-annot addr=0x162660
 * Purpose: Splice: sets job +0xAC widget oid, +0xB0 intro label, +0xB4 outro label used by the completion
 *   sequence
 * Info: proposed name SetJobCompletionWidget (T2); TU game
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r3b_rest
 * See: docs/reference/functions/game-progression.md#game-0x1613a8-0x166270
 * @end-sly2-annot */
void func_00162660(struct S *p, int a, int b, int c)
{
    p->a = a;
    p->b = b;
    p->c = c;
}
