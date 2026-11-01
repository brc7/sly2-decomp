void SetStepplayerLeanTarget(void *p, int a, int b);
void ForceStepplayerLeanTarget(char *p, int a, int b)
{
    SetStepplayerLeanTarget(p, a, b);
    *(int *)(p + 0x1100) = 1;
}
