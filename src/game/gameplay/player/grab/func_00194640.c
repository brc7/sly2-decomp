struct S { int a, b, k, c; };
static __inline__ int FX(struct S *p) { return p->c == 9 || p->c == 10; }
/** @sly2-annot addr=0x194640
 * Purpose: True for grab kind 0xC, or kind 8 when helper says so
 * Info: proposed name FJtGrabBusy (T2); TU grab
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r4b_rest
 * See: docs/reference/functions/player.md#grab-0x191090-0x195950
 * @end-sly2-annot */
int func_00194640(struct S *p) {
    switch (p->k) {
    case 8: if (FX(p)) goto one; break; case 0xC: one: return 1;
    }
    return 0;
}
