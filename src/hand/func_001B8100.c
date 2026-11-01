struct S { char pad[0x13D0]; int n; char pad2[0x17F4 - 0x13D4]; int f17f4; };
int func_001409B8(struct S *p);
/** @sly2-annot addr=0x1B8100
 * Purpose: MURRAY vt98: base 0x1409B8; if -1 return 1 for states 1/12 or strafe with substate set
 * Info: proposed name MURRAY_vt98 (T2); class MURRAY slot98; TU murray
 * Confidence: low (draft) [T2 0.25] - evidence: static analysis; sources: r5b_rest
 * See: docs/reference/functions/player.md#murray-0x1b4728-0x1b9f80
 * @end-sly2-annot */
int func_001B8100(struct S *p)
{
    int v = func_001409B8(p);
    if (v != -1) return v;
    switch (p->n) {
    case 1: case 12:
        return 1;
    case 9:
        if (p->f17f4) v = 1;
        return v;
    }
    return -1;
}
