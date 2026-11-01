struct CLOCK { float t; float dt; };
extern CLOCK D_002DDED0 asm("D_002DDED0");
enum EYESS { EYESS_Nil = -1, EYESS_Open = 0, EYESS_Closing = 1, EYESS_Closed = 2, EYESS_Opening = 3 };
struct SAI { int a; };
struct EYES { char pad[0x1C]; int cframe; char pad1[0x34 - 0x20]; SAI sai; char pad2[0x50 - 0x38]; float dtBlink; float dtOpenMin; float dtOpenMax; float uDoubleBlink;
    int pad60; EYESS eyess; float tEyess; float dtOpen; float dframe; float iframe; float uOpen; };
extern "C" int func_0011A1E8(SAI *psai);
extern "C" void func_001EDA68(EYES *p, int iframe);
void SetEyesEyess(EYES *peyes, EYESS eyess);
extern "C" void func_001EE228(EYES *peyes);
/** @sly2-annot addr=0x1EE228
 * Purpose: EYES vt8 Update
 * Info: proposed name UpdateEyes (T1); class EYES slot8; TU shdanim
 * Confidence: mid (draft) [T1 0.60] - evidence: static analysis; sources: r6_1D5150
 * See: docs/reference/functions/rendering.md#shdanim-0x1ed7a8-0x1ee988
 * @end-sly2-annot */
void func_001EE228(EYES *peyes)
{
    if (func_0011A1E8(&peyes->sai) || peyes->cframe < 2)
        return;

    EYESS eyessNext = peyes->eyess;
    switch (peyes->eyess)
    {
    case EYESS_Open:
        if (D_002DDED0.t - peyes->tEyess >= peyes->dtOpen)
            eyessNext = EYESS_Closing;
        break;
    case EYESS_Closing:
    {
        float dframe = peyes->dframe * D_002DDED0.dt;
        peyes->iframe += dframe;
        if (peyes->iframe >= (float)peyes->cframe)
        {
            peyes->iframe -= dframe;
            eyessNext = ((int)peyes->iframe == peyes->cframe - 1) ? EYESS_Closed : EYESS_Opening;
        }
        break;
    }
    case EYESS_Closed:
        if (peyes->uOpen < 1.0f)
            eyessNext = EYESS_Opening;
        break;
    case EYESS_Opening:
        peyes->iframe -= peyes->dframe * D_002DDED0.dt;
        if (peyes->iframe <= peyes->cframe * peyes->uOpen)
            eyessNext = EYESS_Open;
        break;
    }

    SetEyesEyess(peyes, eyessNext);
    func_001EDA68(peyes, (int)peyes->iframe);
}
