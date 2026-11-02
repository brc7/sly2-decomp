struct S { char pad[0x1A0]; float g1a0; char pad2[0x2C]; float g1d0, g1d4, g1d8, g1dc, g1e0, g1e4; };
void InitDprize(struct S *p);
/** @sly2-annot addr=0x1C6AB8
 * Purpose: CID 92 Init
 * Info: proposed name InitKey (T2); class KEY slot7; TU prize
 * Notes: class identity from whole_game WE names/object strings: InitDprize92 -> InitKey
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r5_1AE130+whole_game(class
 *   identity)
 * See: docs/reference/functions/game-progression.md#prize-0x1c4e50-0x1c8cc0
 * @end-sly2-annot */
void func_001C6AB8(struct S *p)
{
    InitDprize(p);
    p->g1d8 = 35.0f; //P
    p->g1dc = 35.0f; //P
    p->g1d0 = 500.0f; //P
    p->g1d4 = 250.0f; //P
    p->g1e4 = 0.6f; //P
    p->g1e0 = 0.6f; //P
    p->g1a0 = 0.75f; //P
}
