struct S { char pad[0x204]; int f204; char pad2[0x13d0 - 0x208]; int st; };
int func_00143C78(struct S *);
/** @sly2-annot addr=0x136CC0
 * Purpose: BENTLEY vt85: per-state mode code, else STEPPLAYER::VUnk84
 * Info: proposed name BENTLEY_VUnk84 (T2); class BENTLEY slot85; TU bentley
 * Confidence: low (draft) [T2 0.35] - evidence: static analysis; sources: r2b_rest
 * See: docs/reference/functions/player.md#bentley-0x134648-0x1378d8
 * @end-sly2-annot */
int func_00136CC0(struct S *p) {
    if (p->f204) return 0;
    switch (p->st) {
    case 13: return 0;
    case 8: return 5;
    case 14: return 0xf;
    }
    return func_00143C78(p);
}
