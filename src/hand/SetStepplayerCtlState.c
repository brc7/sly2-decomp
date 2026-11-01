void SetPoCtlState(void *p, int n);
void func_001F82B8(void *p, int a, int b);
void ClearBhvstk__FP6BHVSTK(void *p);
void SetStepplayerCtlState(char *p, int n)
{
    SetPoCtlState(p, n);
    func_001F82B8(p, n == 1 ? 0 : -1, 0);
    if (n == 1) ClearBhvstk__FP6BHVSTK(p + 0x26C);
}
