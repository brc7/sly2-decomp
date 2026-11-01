struct O { char pad[0x40]; int f; };
struct S { char pad[0x16C]; struct O *a; struct O *b; struct O *c; };
void func_001F36A8(struct S *p);
static __inline__ void ClearBit(struct O *po)
{
    int f = po->f;
    if ((f ^ 1) & 1) {
        *(int *)f &= ~8;
    } else {
        po->f = f & ~8;
    }
}
/** @sly2-annot addr=0x229F88
 * Purpose: SO PostLoad then clears grf bit 8 on child markers
 * Info: proposed name PostWayptLoad (T1); class WAYPT slot15; TU waypoint
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: r8_21BB88
 * See: docs/reference/functions/game-progression.md#waypoint-0x229ef0-0x22b040
 * @end-sly2-annot */
void func_00229F88(struct S *p)
{
    func_001F36A8(p);
    if (p->a) ClearBit(p->a);
    if (p->b) ClearBit(p->b);
    if (p->c) ClearBit(p->c);
}
