struct E { int a[6]; };
void TriggerStateRsmg(void *p, struct E *pe, void *q, int n);
/** @sly2-annot addr=0x1623D0
 * Purpose: Loops TriggerStateRsmg over an array of 24-byte records
 * Info: proposed name TriggerStateRsmgs (T1); TU game
 * Notes: names_v2 v1 proposed InitTurret__FP6TURRET (0.017)
 * Confidence: mid (draft) [T1 0.55] - evidence: static naming pass; sources: r3b_rest
 * See: docs/reference/functions/game-progression.md#game-0x1613a8-0x166270
 * @end-sly2-annot */
void TriggerStateRsmgs(void *p, void *q, int c, struct E *ae, int n)
{
    int i;
    for (i = 0; i < c; i++)
        TriggerStateRsmg(p, &ae[i], q, n);
}
