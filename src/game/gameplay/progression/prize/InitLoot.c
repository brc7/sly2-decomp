struct S { char pad[0x1A0]; float a1A0; char p1[0x1D0 - 0x1A4]; float a1D0, a1D4, a1D8, a1DC, a1E0, a1E4; char p2[0x230 - 0x1E8]; float a230, a234; };
void InitDprize(struct S *p);
/** @sly2-annot addr=0x1C7648
 * Purpose: CID 94 Init
 * Info: proposed name InitLoot (T1); class LOOT slot7; TU prize
 * Confidence: mid (draft) [T1 0.55] - evidence: static analysis; sources: r5_1AE130
 * See: docs/reference/functions/game-progression.md#prize-0x1c4e50-0x1c8cc0
 * @end-sly2-annot */
void InitLoot(struct S *p)
{
    InitDprize(p);
    p->a1D8 = 35.0f;
    p->a1DC = 40.0f;
    p->a1D0 = 250.0f;
    p->a1D4 = 50.0f;
    p->a1E4 = 0.6f;
    p->a1E0 = 0.75f;
    p->a230 = 8.0f;
    p->a234 = 10.0f;
}
