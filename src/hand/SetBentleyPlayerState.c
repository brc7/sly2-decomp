void SetBentleyState(void *, int);
/** @sly2-annot addr=0x135C20
 * Purpose: BENTLEY vt88: map generic player state 0..8 to Bentley states (0,2,3,4,9,10,11,12,13) via
 *   SetBentleyState
 * Info: proposed name SetBentleyPlayerState (T1); class BENTLEY slot88; TU bentley
 * Confidence: mid (draft) [T1 0.60] - evidence: static analysis; sources: r2b_rest
 * See: docs/reference/functions/player.md#bentley-0x134648-0x1378d8
 * @end-sly2-annot */
void SetBentleyPlayerState(void *p, unsigned int n) {
    switch (n) {
    case 0: SetBentleyState(p, 0); break;
    case 1: SetBentleyState(p, 2); break;
    case 2: SetBentleyState(p, 3); break;
    case 3: SetBentleyState(p, 4); break;
    case 4: SetBentleyState(p, 9); break;
    case 5: SetBentleyState(p, 10); break;
    case 6: SetBentleyState(p, 11); break;
    case 7: SetBentleyState(p, 12); break;
    case 8: SetBentleyState(p, 13); break;
    }
}
