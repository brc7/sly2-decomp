extern struct CLOCK { float t; } D_002DDED0;
extern int D_002C2248[];
struct S { char p0[0x150]; int n150; float g154; char p1[0x164-0x158]; void *p164; };
void SetSmaGoalIfReachable(void *p, int n);
/** @sly2-annot addr=0x1D4FA0
 * Purpose: Pickup state x150 with time; SMA goal from D_002C2248[state]
 * Info: proposed name SetRcPickupState (T1); TU tu_1D1370
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: r5b_rest
 * See: docs/reference/functions/gameplay-objects.md#tu_1d1370-0x1d1370-0x1d5150
 * @end-sly2-annot */
void SetRcPickupState(struct S *p, int n) {
    if (n == p->n150) return;
    p->n150 = n;
    p->g154 = D_002DDED0.t;
    if (p->p164 && n != -1) SetSmaGoalIfReachable(p->p164, D_002C2248[n]);
}
