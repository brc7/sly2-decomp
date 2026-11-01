void func_00135248(void *, int);
/** @sly2-annot addr=0x135C20
 * Purpose: BENTLEY vt88: map generic player state 0..8 to Bentley states (0,2,3,4,9,10,11,12,13) via
 *   SetBentleyState
 * Info: proposed name SetBentleyPlayerState (T1); class BENTLEY slot88; TU bentley
 * Confidence: mid (draft) [T1 0.60] - evidence: static analysis; sources: r2b_rest
 * See: docs/reference/functions/player.md#bentley-0x134648-0x1378d8
 * @end-sly2-annot */
void func_00135C20(void *p, unsigned int n) {
    switch (n) {
    case 0: func_00135248(p, 0); break;
    case 1: func_00135248(p, 2); break;
    case 2: func_00135248(p, 3); break;
    case 3: func_00135248(p, 4); break;
    case 4: func_00135248(p, 9); break;
    case 5: func_00135248(p, 10); break;
    case 6: func_00135248(p, 11); break;
    case 7: func_00135248(p, 12); break;
    case 8: func_00135248(p, 13); break;
    }
}
