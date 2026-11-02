struct CLOCK { float t; float dt; };
extern CLOCK D_002DDED0 asm("D_002DDED0");
struct SAI { int a; };
struct SHUFFLE { char pad[0x1C]; int cframe; char pad1[0x28 - 0x20]; unsigned short iframe; char pad2[0x34 - 0x2A]; SAI sai;
    char pad3[0x50 - 0x38]; float dtPauseMin; float dtPauseMax; float dtPause; };
extern "C" int FIsSlEmpty(SAI *psai);
extern "C" int NRandInRange__Fii(int nLow, int nHigh);
extern "C" float GRandInRange__Fff(float gLow, float gHigh);
extern "C" void SetSaiIframe(SHUFFLE *p, int iframe);
/** @sly2-annot addr=0x1EDFE0
 * Purpose: not written up yet; only the symbol name (pre-existing / Sly 1-derived) describes it.
 * Confidence: mid (draft) - evidence: pre-existing symbol name only; behaviour not reviewed
 * See: docs/reference/functions/rendering.md#shdanim-0x1ed7a8-0x1ee988
 * @end-sly2-annot */
void UpdateShuffle(SHUFFLE *pshuffle, float dt)
{
    if (FIsSlEmpty(&pshuffle->sai) || pshuffle->cframe < 2)
        return;

    if (pshuffle->dtPause > 0.0f)
    {
        pshuffle->dtPause -= D_002DDED0.dt;
        return;
    }

    int randFrame = NRandInRange__Fii(1, pshuffle->cframe - 1);
    int newIframe = (pshuffle->iframe + randFrame) % pshuffle->cframe;
    SetSaiIframe(pshuffle, newIframe);
    pshuffle->dtPause = GRandInRange__Fff(pshuffle->dtPauseMin, pshuffle->dtPauseMax);
}
