struct MSG { int cb; int nKind; };
struct G { char pad[0x45CC]; void *p45cc; };
int func_00117C80(void *pbis);
void *func_001AE2A0(struct MSG *pmsg);
void LoadZoneBspFromBrx(void *p, int n, void *pbis);
void LoadSwZoneBspFromBrx(struct G *p, void *pbis)
{
    int n = func_00117C80(pbis);
    if (n) {
        struct MSG msg;
        msg.cb = 0x30;
        msg.nKind = 0x10;
        p->p45cc = func_001AE2A0(&msg);
        LoadZoneBspFromBrx(p->p45cc, n, pbis);
    }
}
