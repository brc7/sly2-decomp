struct S { char pad[0x1A0]; float a1A0; char p1[0x1D0 - 0x1A4]; float a1D0, a1D4, a1D8, a1DC, a1E0, a1E4; char p2[0x230 - 0x1E8]; float a230, a234; };
void InitDprize(struct S *p);
/** @sly2-annot addr=0x1C6508
 * Purpose: CID 91 Init (sprayable pickup; coin?)
 * Info: proposed name InitDprize91 (T2); class COIN slot7; TU prize
 * Confidence: low (draft) [T2 0.45] - evidence: static analysis; sources: r5_1AE130
 * See: docs/reference/functions/game-progression.md#prize-0x1c4e50-0x1c8cc0
 * @end-sly2-annot */
void func_001C6508(struct S *p)
{
    InitDprize(p);
    p->a1D8 = 45.0f;
    p->a1DC = 65.0f;
    p->a1D0 = 250.0f;
    p->a1D4 = 150.0f;
    p->a1E0 = 0.5f;
    p->a1E4 = 0.25f;
    p->a1A0 = 0.25f;
    p->a230 = 5.0f;
    p->a234 = 5.0f;
}
