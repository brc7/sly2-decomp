struct S { char pad[0x13d0]; int st; };
int func_001409B8(struct S *);
/** @sly2-annot addr=0x135E40
 * Purpose: BENTLEY vt98: STEPPLAYER::VUnk97, else 1 in states 1/5/14, -1 otherwise
 * Info: proposed name BENTLEY_VUnk97 (T2); class BENTLEY slot98; TU bentley
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r2b_rest
 * See: docs/reference/functions/player.md#bentley-0x134648-0x1378d8
 * @end-sly2-annot */
int func_00135E40(struct S *p) {
    int r = func_001409B8(p);
    if (r != -1) return r;
    switch (p->st) {
    case 1: case 5: case 14: return 1;
    }
    return -1;
}
