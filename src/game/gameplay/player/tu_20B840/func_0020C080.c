int FLeapDone(char *p);
void SetLeapState(char *p, int n);
void func_001305B8(int a, int b, int c, void *pv, float g, float h);
/** @sly2-annot addr=0x20C080
 * Purpose: if not finished: state 4, clear anim job
 * Info: proposed name StopLeap (T2); TU tu_20B840
 * Confidence: low (draft) [T2 0.45] - evidence: static analysis; sources: r7c_hud
 * See: docs/reference/functions/animation.md#tu_20b840-0x20b840-0x20ce28
 * @end-sly2-annot */
void func_0020C080(char *p) { if (!FLeapDone(p)) { SetLeapState(p, 4); func_001305B8(0, 0, 0, p + 0x38, 0.0f, 1.0f); } }
