extern struct CLOCK { float a[16]; } D_002DDED0;
struct S { char p0[0x40]; int n40; float g44; char p1[0x11C-0x48]; int n11C, n120, n124; };
/** @sly2-annot addr=0x1DCD08
 * Purpose: Job list: state -1, timestamp, anim handles -1
 * Info: proposed name ClearJobList (T1); TU screen
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: r9_bhv_screen
 * See: docs/reference/functions/hud-ui.md#screen-0x1d93d8-0x1de2a0
 * @end-sly2-annot */
void func_001DCD08(struct S *p) {
    p->n40 = -1; //P
    p->g44 = D_002DDED0.a[15]; //P
    p->n11C = -1; //P
    p->n120 = -1; //P
    p->n124 = -1; //P
}
