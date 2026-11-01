struct G;
extern struct G D_003DD490;
extern char D_002DDED0[], D_002DEB30[];
struct S { char pad[0x24]; int x24; };
void func_001D9910(struct S *p);
void func_0014D1D0(void *p, int n, float g);
void DecrementSwHandsOff(struct G *p, int n);
void BroadcastGameEvent(void *p, int n, int m);
void func_001AEA40(struct S *p)
{
    func_001D9910(p);
    p->x24 = 0;
    func_0014D1D0(D_002DDED0, 0, 1.0f);
    DecrementSwHandsOff(&D_003DD490, 0);
    BroadcastGameEvent(D_002DEB30, 0x2C, 0);
}
