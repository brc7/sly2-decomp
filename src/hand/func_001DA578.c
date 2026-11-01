struct S { char pad[0x18]; int n18; int n1C; int n20; };
/** @sly2-annot addr=0x1DA578
 * Purpose: Cancel notice n (current -> timeout 0, or drop the queued one); -1 cancels the queue
 * Info: proposed name CancelNotice (T1); TU screen
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: r9_bhv_screen
 * See: docs/reference/functions/hud-ui.md#screen-0x1d93d8-0x1de2a0
 * @end-sly2-annot */
void func_001DA578(struct S *p, int n) {
    if (n == -1) {
        p->n20 = -1;
        p->n1C = 0;
    } else if (n == p->n18) {
        p->n1C = 0;
    } else if (n == p->n20) {
        p->n20 = -1;
    }
}
