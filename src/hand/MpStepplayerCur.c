int MpStepplayerCur(char *p)
{
    int *q = *(int **)(p + 0x12FC);
    if (!q) return *(int *)(p + 0x1300);
    return *q;
}
