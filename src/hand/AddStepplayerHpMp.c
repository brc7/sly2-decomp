void AddPoHpMp(void *p, int a, int b);
void SetStepplayerHp(void *p, int a);
int MpStepplayerCur(void *p);
void SetStepplayerMp(void *p, int a);
void AddStepplayerHpMp(char *p, int a, int b)
{
    AddPoHpMp(p, a, b);
    SetStepplayerHp(p, **(int **)(p + 0xE00) + a);
    SetStepplayerMp(p, MpStepplayerCur(p) + b);
}
