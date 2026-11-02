struct S { char pad[0x18C8]; int n; };
/** @sly2-annot addr=0x18D620
 * Purpose: JT vt89: inverse map jts -> generic state 0..8 or -1
 * Info: proposed name GetJtPlayerState (T1); class JT slot89; TU jt
 * Confidence: mid (draft) [T1 0.55] - evidence: static analysis; sources: r4_17F090
 * See: docs/reference/functions/player.md#jt-0x188488-0x18e540
 * @end-sly2-annot */
int GetJtPlayerState(struct S *p)
{
    switch (p->n) {
    case 0: return 0;
    case 2: return 1;
    case 7: return 2;
    case 4: return 3;
    case 18: return 4;
    case 19: return 5;
    case 20: return 6;
    case 23: return 7;
    case 8: return 8;
    }
    return -1;
}
