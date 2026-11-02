struct S { char p0[0x1A0]; float x1A0; char p1[0x1D0 - 0x1A4]; float x1D0, x1D4, x1D8, x1DC, x1E0, x1E4;
    char p2[0x230 - 0x1E8]; float x230, x234; int x238; int x23C; int x240; };
void InitDprize(struct S *p);
/** @sly2-annot addr=0x1C67D8
 * Purpose: CID 93 Init (sprayable pickup 0x5D)
 * Info: proposed name InitMedicine (T2); class MEDICINE slot7; TU prize
 * Notes: class identity from whole_game WE names/object strings: InitDprize93 -> InitMedicine
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r5_1AE130+whole_game(class
 *   identity)
 * See: docs/reference/functions/game-progression.md#prize-0x1c4e50-0x1c8cc0
 * @end-sly2-annot */
void func_001C67D8(struct S *p)
{
    InitDprize(p);
    p->x1D8 = 45.0f;
    p->x1DC = 75.0f;
    p->x1D0 = 250.0f;
    p->x1D4 = 50.0f;
    p->x1E4 = 0.2f;
    p->x1E0 = 0.65f;
    p->x1A0 = 0.25f;
    p->x23C = 10;
    p->x240 = 20;
    p->x230 = 4.0f;
    p->x234 = 5.0f;
}
