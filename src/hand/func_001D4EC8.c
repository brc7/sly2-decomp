struct S { char p0[0x160]; void *p160; int a164; };
void func_001F36A8(struct S *p);
void func_001F1448(void *a, struct S *p, int b, int c, int *pn);
void func_001D4FA0(struct S *p, int n);
/** @sly2-annot addr=0x1D4EC8
 * Purpose: CID 108 vt15: PostSoLoad, SMA binding, state 0
 * Info: proposed name PostRcPickupLoad (T1); class CID108 slot15; TU tu_1D1370
 * Confidence: mid (draft) [T1 0.60] - evidence: static analysis; sources: r5b_rest
 * See: docs/reference/functions/gameplay-objects.md#tu_1d1370-0x1d1370-0x1d5150
 * @end-sly2-annot */
void func_001D4EC8(struct S *p) {
    func_001F36A8(p);
    func_001F1448(p->p160, p, -1, 0, &p->a164);
    func_001D4FA0(p, 0);
}
