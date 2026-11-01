struct E { int a, b, c, d; int pad[4]; };
extern struct E D_002BCD00[];
extern float D_002DDED0;
int MpStepplayerCur(void *p);
void SetStepplayerMp(void *p, int a);
void ChargeGadgetMp(char *p, int i)
{
    if (i != -1) SetStepplayerMp(p, MpStepplayerCur(p) - D_002BCD00[i].d);
    *(float *)(p + 0x1304) = D_002DDED0;
}
