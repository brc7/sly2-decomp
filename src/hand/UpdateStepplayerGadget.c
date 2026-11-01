extern float D_002BD260;
extern float D_002BD264;
void DrainStepplayerMp(void *p, float f);
void UpdateStepplayerGadget(void *p, int *pm)
{
    switch (*pm) {
    case 0x1D:
        DrainStepplayerMp(p, D_002BD260);
        break;
    case 0x2A:
        DrainStepplayerMp(p, D_002BD264);
        break;
    }
}
