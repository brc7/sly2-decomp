struct CLOCK { float t; float dt; };
extern CLOCK D_002DDED0 asm("D_002DDED0");
struct SAI { int a; };
struct SHUFFLE { char pad[0x1C]; int cframe; char pad1[0x28 - 0x20]; unsigned short iframe; char pad2[0x34 - 0x2A]; SAI sai;
    char pad3[0x50 - 0x38]; float dtPauseMin; float dtPauseMax; float dtPause; };
extern "C" int FIsSlEmpty(SAI *psai);
extern "C" int NRandInRange__Fii(int nLow, int nHigh);
extern "C" float GRandInRange__Fff(float gLow, float gHigh);
extern "C" void func_001EDA68(SHUFFLE *p, int iframe);
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
    func_001EDA68(pshuffle, newIframe);
    pshuffle->dtPause = GRandInRange__Fff(pshuffle->dtPauseMin, pshuffle->dtPauseMax);
}
