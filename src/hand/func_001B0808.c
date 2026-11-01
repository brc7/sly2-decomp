extern char D_002DEB30[];
extern void *D_003E0F9C;
extern char D_003DA3C0[];
void BroadcastGameEvent(void *p, int n, void *q);
void func_001DD300(void *p, int f);
void func_001B0808(void)
{
    BroadcastGameEvent(D_002DEB30, 0x2B, 0);
    func_001DD300(D_003DA3C0, D_003E0F9C != 0);
}
