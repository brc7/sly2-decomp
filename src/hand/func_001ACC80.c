struct S { char pad[0x40]; int x40; };
/** @sly2-annot addr=0x1ACC80
 * Purpose: State -> 1 (one-button) / 2 (two-choice) / 0
 * Info: proposed name KMcDialogButtons (T2); TU memcard
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r4_17F090
 * See: docs/reference/functions/save-memcard.md#memcard-0x1aa978-0x1ae150
 * @end-sly2-annot */
int func_001ACC80(struct S *p)
{
    switch (p->x40) {
    case 0x11: case 0x14: case 0x17: case 0x18: case 0x1B: case 0x1D: case 0x22: case 0x23: case 0x24: case 0x2E:
        return 1;
    case 0x12: case 0x15: case 0x1C: case 0x20:
        return 2;
    }
    return 0;
}
