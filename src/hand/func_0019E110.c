struct C { char pad[0xE4]; int xe4; };
struct S { int a, b, c; struct C *pc; int d, e; int x18; };
float func_001E6620(struct C *);
float func_001E6648(struct C *);
/** @sly2-annot addr=0x19E110
 * Purpose: Allowed s range on the pipe curve: unbounded if closed, else [sMin+35 or +100, sMax-100]
 * Info: proposed name GetJtPipeSRange (T2); TU pipe_jt
 * Confidence: low (draft) [T2 0.45] - evidence: static analysis; sources: r4b_rest
 * See: docs/reference/functions/player.md#pipe_jt-0x19de10-0x19f0a8
 * @end-sly2-annot */
void func_0019E110(struct S *p, float *ag) {
    if (p->pc->xe4) {
        ag[0] = -3.40282346638528859812e+38f;
        ag[1] = 3.40282346638528859812e+38f;
    } else if (p->x18) {
        ag[0] = func_001E6620(p->pc) + 35.0f;
        ag[1] = func_001E6648(p->pc) - 100.0f;
    } else {
        ag[0] = func_001E6620(p->pc) + 100.0f;
        ag[1] = func_001E6648(p->pc) - 35.0f;
    }
}
