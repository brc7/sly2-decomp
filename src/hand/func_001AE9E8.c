struct G;
extern struct G D_003DD490;
extern char D_002DDED0[], D_002DEB30[];
void func_001D98B0(void);
void func_0014D1D0(void *p, int n, float g);
void IncrementSwHandsOff(struct G *p, int n);
void BroadcastGameEvent(void *p, int n, int m);
void func_001AE9E8(void)
{
    func_001D98B0();
    func_0014D1D0(D_002DDED0, 0, 0.0f);
    IncrementSwHandsOff(&D_003DD490, 0);
    BroadcastGameEvent(D_002DEB30, 0x28, 0);
}
