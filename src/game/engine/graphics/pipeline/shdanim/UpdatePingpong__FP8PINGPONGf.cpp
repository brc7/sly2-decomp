struct CLOCK { float t; float dt; };
extern CLOCK D_002DDED0 asm("D_002DDED0");
struct SAI { int a; };
struct PINGPONG { char pad[0x1C]; int cframe; char pad1[0x34 - 0x20]; SAI sai; char pad2[0x50 - 0x38];
    float dtLoopMin; float dtLoopMax; float dtPauseMin; float dtPauseMax; float dframe; float iframe; float dtPause; float dtPauseRemaining; };
extern "C" int FIsSlEmpty(SAI *psai);
extern "C" float GRandInRange__Fff(float gLow, float gHigh);
extern "C" float GModPositive__Fff(float g, float gMod);
extern "C" void SetSaiIframe(PINGPONG *p, int iframe);
/** @sly2-annot addr=0x1EDE10
 * Purpose: not written up yet; only the symbol name (pre-existing / Sly 1-derived) describes it.
 * Confidence: mid (draft) - evidence: pre-existing symbol name only; behaviour not reviewed
 * See: docs/reference/functions/rendering.md#shdanim-0x1ed7a8-0x1ee988
 * @end-sly2-annot */
void UpdatePingpong(PINGPONG *ppingpong, float dt)
{
    if (FIsSlEmpty(&ppingpong->sai) || ppingpong->cframe < 2)
        return;

    if (ppingpong->dtPauseRemaining > 0.0f)
    {
        ppingpong->dtPauseRemaining -= D_002DDED0.dt;
        return;
    }

    float dframe = ppingpong->dframe * D_002DDED0.dt;
    ppingpong->iframe += dframe;

    if (ppingpong->iframe >= (float)ppingpong->cframe)
    {
        ppingpong->iframe -= dframe;
        ppingpong->dframe = -ppingpong->dframe;
    }

    if (ppingpong->iframe < 0.0f)
    {
        ppingpong->iframe = 0.0f;
        float rand1 = GRandInRange__Fff(ppingpong->dtLoopMin, ppingpong->dtLoopMax);
        ppingpong->dframe = (float)(ppingpong->cframe * 2) / rand1;

        float rand2 = GRandInRange__Fff(ppingpong->dtPauseMin, ppingpong->dtPauseMax);
        ppingpong->dtPause = rand2;
        ppingpong->dtPauseRemaining = rand2;
    }

    SetSaiIframe(ppingpong, (int)ppingpong->iframe);
}
