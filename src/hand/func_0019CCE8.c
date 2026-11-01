struct S { char pad[0x38]; int *p; };
void func_0019CD38(struct S *, int);
/** @sly2-annot addr=0x19CCE8
 * Purpose: Leave the mode (SetJtModeState 2) unless the owner state is 0/8/9/10
 * Info: proposed name EndJtMode (T1); TU mode
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: r4b_rest
 * See: docs/reference/functions/cutscenes.md#relay-0x1dfad8-0x1e5d80,
 *   docs/reference/functions/player.md#bentley-0x134648-0x1378d8,
 *   docs/reference/functions/player.md#jt-0x188488-0x18e540
 * @end-sly2-annot */
void func_0019CCE8(struct S *p) {
    if (p->p) {
        switch (*p->p) {
        case 0: case 8: case 9: case 10: return;
        }
    }
    func_0019CD38(p, 2);
}
