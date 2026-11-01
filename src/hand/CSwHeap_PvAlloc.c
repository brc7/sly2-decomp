typedef struct MSG { int n; int nKind; } MSG;
void func_001AE1D0(MSG *pmsg);

void CSwHeap_PvAlloc(void *pv, int n)
{
    MSG msg;
    msg.n = n;
    msg.nKind = 0x10;
    func_001AE1D0(&msg);
}
