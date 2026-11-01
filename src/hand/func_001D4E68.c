struct S { char p0[0x40]; int x40; char p1[0x150-0x44]; int n150; int a[2]; int n15C; };
void func_001F2108(struct S *p);
/** @sly2-annot addr=0x1D4E68
 * Purpose: CID 108 vt7 Init: InitSo, amount 30 (x15C), state -1
 * Info: proposed name InitRcPickup (T1); class CID108 slot7; TU tu_1D1370
 * Confidence: mid (draft) [T1 0.60] - evidence: static analysis; sources: r5b_rest
 * See: docs/reference/functions/gameplay-objects.md#tu_1d1370-0x1d1370-0x1d5150
 * @end-sly2-annot */
void func_001D4E68(struct S *p) {
    int v;
    func_001F2108(p);
    p->n15C = 0x1E;
    p->n150 = -1;
    v = p->x40;
    if ((v ^ 1) & 1) *(int *)v |= 2;
    else p->x40 = v | 2;
}
