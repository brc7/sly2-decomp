struct CLOCK { float t; float dt; };
extern CLOCK D_002DDED0 asm("D_002DDED0");
struct SAI { int a; };
struct SHUFFLE { char pad[0x1C]; int cframe; char pad1[0x28 - 0x20]; unsigned short iframe; char pad2[0x34 - 0x2A]; SAI sai;
    char pad3[0x50 - 0x38]; float dtPauseMin; float dtPauseMax; float dtPause; };
extern "C" int func_0011A1E8(SAI *psai);
extern "C" int func_0011D3F8(int nLow, int nHigh);
extern "C" float func_0011D448(float gLow, float gHigh);
extern "C" void func_001EDA68(SHUFFLE *p, int iframe);
/** @sly2-annot addr=0x1EDFE0
 * Purpose: not written up yet; only the symbol name (pre-existing / Sly 1-derived) describes it.
 * Confidence: mid (draft) - evidence: pre-existing symbol name only; behaviour not reviewed
 * See: docs/reference/functions/rendering.md#shdanim-0x1ed7a8-0x1ee988
 * @end-sly2-annot */
void UpdateShuffle(SHUFFLE *pshuffle, float dt)
{
    if (func_0011A1E8(&pshuffle->sai) || pshuffle->cframe < 2)
        return;

    if (pshuffle->dtPause > 0.0f)
    {
        pshuffle->dtPause -= D_002DDED0.dt;
        return;
    }

    int randFrame = func_0011D3F8(1, pshuffle->cframe - 1);
    int newIframe = (pshuffle->iframe + randFrame) % pshuffle->cframe;
    func_001EDA68(pshuffle, newIframe);
    pshuffle->dtPause = func_0011D448(pshuffle->dtPauseMin, pshuffle->dtPauseMax);
}
