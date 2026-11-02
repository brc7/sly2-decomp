struct S { char pad[0x18D8]; int n; };
int FJtSweepBack(struct S *p);
/** @sly2-annot addr=0x18F050
 * Purpose: Return the next sweep jtcs: toggle 0x16<->0x17, else 0x17 if FJtSweepBack else 0x16
 * Info: proposed name JtcsChooseJtSweep (T2); TU cane
 * Confidence: low (draft) [T2 0.45] - evidence: static analysis; sources: r4b_rest
 * See: docs/reference/functions/player.md#cane-0x18e540-0x18f908
 * @end-sly2-annot */
int func_0018F050(struct S *p)
{
    int r;
    switch (p->n) {
    case 0x16: r = 0x17; break;
    case 0x17: r = 0x16; break;
    default:
        r = FJtSweepBack(p) ? 0x17 : 0x16;
        break;
    }
    return r;
}
