struct S { char pad[0x13d0]; int st; };
/** @sly2-annot addr=0x135D08
 * Purpose: BENTLEY vt89: inverse map Bentley state -> generic 0..8 (else -1)
 * Info: proposed name BENTLEY_PlayerStateCur (T1); class BENTLEY slot89; TU bentley
 * Confidence: mid (draft) [T1 0.60] - evidence: static analysis; sources: r2b_rest
 * See: docs/reference/functions/player.md#bentley-0x134648-0x1378d8
 * @end-sly2-annot */
int BENTLEY_PlayerStateCur(struct S *p) {
    switch (p->st) {
    case 0: return 0;
    case 2: return 1;
    case 3: return 2;
    case 4: return 3;
    case 9: return 4;
    case 10: return 5;
    case 11: return 6;
    case 12: return 7;
    case 13: return 8;
    }
    return -1;
}
