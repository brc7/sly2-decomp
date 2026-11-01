void ResetRhythmScore(char *p)
{
    *(int *)(p + 0x23C) = 0;
    *(int *)(p + 0x284) = 0;
    *(int *)(p + 0x288) = 0;
}
