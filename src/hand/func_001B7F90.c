struct S { char pad[0x13D0]; int n; };
/** @sly2-annot addr=0x1B7F90
 * Purpose: MURRAY vt89: Murray state -> generic player state 0..8 or -1
 * Info: proposed name GetMurrayPlayerState (T1); class MURRAY slot89; TU murray
 * Notes: runtime3/4: generic state map (0/5->0, 13->2) observed
 * Confidence: mid (draft) [T1 0.60] - evidence: runtime (save-state RAM); sources: r5b_rest+runtime
 * See: docs/reference/functions/player.md#murray-0x1b4728-0x1b9f80
 * @end-sly2-annot */
int func_001B7F90(struct S *p)
{
    switch (p->n) {
    case 0: case 5: return 0;
    case 2: return 1;
    case 13: return 2;
    case 14: return 3;
    case 16: return 4;
    case 17: return 5;
    case 18: return 6;
    case 19: return 7;
    case 20: return 8;
    }
    return -1;
}
