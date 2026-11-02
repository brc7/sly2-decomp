struct S { char pad[0x1148]; int a; float f; };
/** @sly2-annot addr=0x142868
 * Purpose: Set kind a and t = FLT_MAX
 * Info: proposed name InitIdleTimer (T2); TU stepplayer
 * Confidence: low (draft) [T2 0.35] - evidence: static analysis; sources: r2b_rest
 * See: docs/reference/functions/player.md#stepplayer-0x13ff70-0x144638
 * @end-sly2-annot */
void func_00142868(struct S *p, int a)
{
    p->a = a;
    p->f = 3.40282346638528859812e+38f;
}
