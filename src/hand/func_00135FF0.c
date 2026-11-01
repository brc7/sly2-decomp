struct S { char pad[0x13d0]; int st; };
/** @sly2-annot addr=0x135FF0
 * Purpose: BENTLEY vt94: 1 in states 0/1/5/7 else 0
 * Info: proposed name BENTLEY_VUnk93 (T2); class BENTLEY slot94; TU bentley
 * Confidence: low (draft) [T2 0.35] - evidence: static analysis; sources: r2b_rest
 * See: docs/reference/functions/player.md#bentley-0x134648-0x1378d8
 * @end-sly2-annot */
int func_00135FF0(struct S *p) {
    switch (p->st) {
    case 0: case 1: case 5: case 7: return 1;
    }
    return 0;
}
