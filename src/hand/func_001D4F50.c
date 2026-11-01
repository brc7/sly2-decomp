struct O { char pad[0x98]; short h98; };
struct S { char p0[0x58]; struct O *po; char p1[0x150-0x5C]; int n150; };
void func_001F36E0(struct S *p);
void func_001D4FA0(struct S *p, int n);
/** @sly2-annot addr=0x1D4F50
 * Purpose: CID 108 vt16: UpdateSo; in state 0 go to state 1 when owner's x98 == 3
 * Info: proposed name UpdateRcPickup (T1); class CID108 slot16; TU tu_1D1370
 * Confidence: mid (draft) [T1 0.55] - evidence: static analysis; sources: r5b_rest
 * See: docs/reference/functions/gameplay-objects.md#tu_1d1370-0x1d1370-0x1d5150
 * @end-sly2-annot */
void func_001D4F50(struct S *p) {
    func_001F36E0(p);
    if (p->n150 == 0 && p->po->h98 == 3) func_001D4FA0(p, 1);
}
