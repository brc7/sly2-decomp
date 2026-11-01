struct S { char pad[0x1C4]; int x; };
void func_001F2D48(struct S *p);
void func_001305B8(int a, int b, int c, void *d, float f, float g);
/** @sly2-annot addr=0x1B0C68
 * Purpose: CID 96 vt12: RemoveSoHierarchy then stop animation (ApplyAsegCur null)
 * Info: proposed name RemoveCid96Hierarchy (T2); class CID96 slot12; TU tu_1B0B78
 * Notes: slot 12 is RemoveHierarchy (CONFLICTS#5: LO base 0x1A5800/0x1A5848 AddLoHierarchy/RemoveLoHierarchy;
 *   OnAdd/OnRemove are slots 23/22 per arbitration); was OnCid96Remove
 * Confidence: low (draft) [T2 0.50] - evidence: arbitration; sources: r5b_rest
 * See: docs/reference/functions/gameplay-objects.md#tu_1b0b78-0x1b0b78-0x1b1db8
 * @end-sly2-annot */
void func_001B0C68(struct S *p)
{
    func_001F2D48(p);
    func_001305B8(0, 0, 0, &p->x, 0.0f, 0.0f);
}
