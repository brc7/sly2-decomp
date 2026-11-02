int CploFindSwObjectsByClass(void *, int, int, int, int, int *);
void SetStepguardSpecial(int, void *);
/** @sly2-annot addr=0x19F220
 * Purpose: Put every STEPGUARD into guard state x via 0x207030
 * Info: proposed name SetSwGuardsState (T2); TU tu_19F0A8
 * Confidence: low (draft) [T2 0.35] - evidence: static analysis; sources: r4b_rest
 * See: docs/reference/functions/player.md#tu_19f0a8-0x19f0a8-0x19f3e8
 * @end-sly2-annot */
void func_0019F220(void *p, void *x) {
    int ap[32];
    int c, i;
    c = CploFindSwObjectsByClass(p, 0x105, 0x38, 0, 32, ap);
    for (i = 0; i < c; i++) SetStepguardSpecial(ap[i], x);
}
