void SetPoCtlState(void *p, int n);
void func_001F82B8(void *p, int a, int b);
void func_001EECD0(void *p);
void func_001406E8(char *p, int n)
{
    SetPoCtlState(p, n);
    func_001F82B8(p, n == 1 ? 0 : -1, 0);
    if (n == 1) func_001EECD0(p + 0x26C);
}
