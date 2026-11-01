struct M { char pad[0x14]; int n; };
struct S { char pad[0x1A4]; int n1a4; char pad2[0x1C4 - 0x1A8]; struct M *pm; };
void HandleBasicMessage(struct S *p, int msg, struct M *pm);
void func_001B1328(struct S *p, int n);
/** @sly2-annot addr=0x1B10D0
 * Purpose: CID 96 vt5: base handler; msg 2 from its own SMA with matching id -> state 4
 * Info: proposed name HandleCid96Message (T2); class CID96 slot5; TU tu_1B0B78
 * Confidence: low (draft) [T2 0.60] - evidence: static analysis; sources: r5b_rest
 * See: docs/reference/functions/gameplay-objects.md#tu_1b0b78-0x1b0b78-0x1b1db8
 * @end-sly2-annot */
void func_001B10D0(struct S *p, int msg, struct M *pm)
{
    HandleBasicMessage(p, msg, pm);
    if (msg == 2 && pm == p->pm && pm->n == p->n1a4) func_001B1328(p, 4);
}
