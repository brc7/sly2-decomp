struct S { char pad[0x18C8]; int n; };
/** @sly2-annot addr=0x1894F8
 * Purpose: JT vt94 (pure virtual in STEPPLAYER): 1 for jts 0,1,5,10,12,24
 * Info: proposed name JT_vt94 (T2); class JT slot94; TU jt
 * Confidence: low (draft) [T2 0.25] - evidence: static analysis; sources: r4_17F090
 * See: docs/reference/functions/player.md#jt-0x188488-0x18e540
 * @end-sly2-annot */
int func_001894F8(struct S *p)
{
    switch (p->n) {
    case 0: case 1: case 5: case 10: case 12: case 24:
        return 1;
    }
    return 0;
}
