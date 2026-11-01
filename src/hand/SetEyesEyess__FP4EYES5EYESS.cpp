struct CLOCK { float t; float dt; };
extern CLOCK D_002DDED0 asm("D_002DDED0");
enum EYESS { EYESS_Nil = -1, EYESS_Open = 0, EYESS_Closing = 1, EYESS_Closed = 2 };
struct EYES { char pad[0x1C]; int cframe; char pad1[0x50 - 0x20]; float dtBlink; float dtOpenMin; float dtOpenMax; float uDoubleBlink;
    int pad60; EYESS eyess; float tEyess; float dtOpen; float dframe; float iframe; float uOpen; };
extern "C" float GRandInRange__Fff(float gLow, float gHigh);
void SetEyesEyess(EYES *peyes, EYESS eyess)
{
    if (eyess == peyes->eyess)
        return;

    switch (eyess)
    {
    case EYESS_Open:
        peyes->dframe = (peyes->cframe * 2) * (1.0f - peyes->uOpen) / peyes->dtBlink;
        peyes->iframe = (peyes->cframe - 1) * peyes->uOpen;
        if (GRandInRange__Fff(0.0f, 1.0f) < peyes->uDoubleBlink)
            peyes->dtOpen = 0.0f;
        else
            peyes->dtOpen = GRandInRange__Fff(peyes->dtOpenMin, peyes->dtOpenMax);
        break;
    case EYESS_Closing:
        break;
    case EYESS_Closed:
        if ((int)peyes->iframe != peyes->cframe - 1)
            peyes->iframe = peyes->cframe - 1;
        break;
    }

    peyes->eyess = eyess;
    peyes->tEyess = D_002DDED0.t;
}
