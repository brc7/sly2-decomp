struct S { char pad[0x13D0]; int n; };
/** @sly2-annot addr=0x1B8180
 * Purpose: MURRAY vt94: 1 for free-movement states 0/1/9/10
 * Info: proposed name MURRAY_vt94 (T2); class MURRAY slot94; TU murray
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r5b_rest
 * See: docs/reference/functions/player.md#murray-0x1b4728-0x1b9f80
 * @end-sly2-annot */
int func_001B8180(struct S *p)
{
    switch (p->n) {
    case 0: case 1: case 9: case 10:
        return 1;
    }
    return 0;
}
