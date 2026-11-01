struct G { char pad[0x1030]; int x1030; };
struct S { struct G *pg; int n; };
int func_00131968(int);
void func_001A15D0(struct S *);
/** @sly2-annot addr=0x1A12F0
 * Purpose: When the owner's current ASEGA (x1030) has retracted, run the pending post-hurt callback
 *   (0x1A15D0); loop while state changes
 * Info: proposed name UpdateJtHurtInput (T2); TU hurt
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r4b_rest
 * See: docs/reference/functions/player.md#hurt-0x1a0b18-0x1a2bd0
 * @end-sly2-annot */
void func_001A12F0(struct S *p)
{
    int n;
    do {
        n = p->n;
        if (func_00131968(p->pg->x1030))
            func_001A15D0(p);
    } while (p->n != -1 && p->n != n);
}
