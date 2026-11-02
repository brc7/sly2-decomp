struct CLOCK { float t; float dt; };
extern CLOCK D_002DDED0 asm("D_002DDED0");
struct SAI { int a; };
struct LOOP { char pad[0x1C]; int cframe; char pad1[0x34 - 0x20]; SAI sai; char pad2[0x50 - 0x38];
    float dtLoopMin; float dtLoopMax; float dtPauseMin; float dtPauseMax; float dframe; float iframe; float dtPause; float dtPauseRemaining; };
extern "C" int FIsSlEmpty(SAI *psai);
extern "C" float GRandInRange__Fff(float gLow, float gHigh);
extern "C" float GModPositive__Fff(float g, float gMod);
extern "C" void func_001EDA68(LOOP *p, int iframe);
/** @sly2-annot addr=0x1EDC40
 * Purpose: not written up yet; only the symbol name (pre-existing / Sly 1-derived) describes it.
 * Confidence: mid (draft) - evidence: pre-existing symbol name only; behaviour not reviewed
 * See: docs/reference/functions/rendering.md#shdanim-0x1ed7a8-0x1ee988
 * @end-sly2-annot */
void UpdateLoop(LOOP *ploop, float dt)
{
    if (FIsSlEmpty(&ploop->sai))
        return;

    if (ploop->cframe < 2)
        return;

    if (ploop->dtPauseRemaining > 0.0f)
    {
        ploop->dtPauseRemaining -= D_002DDED0.dt;
        return;
    }

    ploop->iframe += ploop->dframe * D_002DDED0.dt;

    if (ploop->iframe >= (float)ploop->cframe)
    {
        float rand1 = GRandInRange__Fff(ploop->dtLoopMin, ploop->dtLoopMax);
        ploop->dframe = (float)ploop->cframe / rand1;

        float rand2 = GRandInRange__Fff(ploop->dtPauseMin, ploop->dtPauseMax);
        ploop->dtPause = rand2;
        ploop->dtPauseRemaining = rand2;
    }

    ploop->iframe = GModPositive__Fff(ploop->iframe, (float)ploop->cframe);
    func_001EDA68(ploop, (int)ploop->iframe);
}
