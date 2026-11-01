struct MSG { int cb; int nKind; };
struct G { char pad[0x45C8]; void *p45c8; void *p45cc; };
void *PloNew__F3CIDP2SO3OIDi(int a, int b, int c, int d);
void func_001C0648(void *p);
void *func_001AE2A0(struct MSG *pmsg);
void func_001C1138(void *p);
void func_001C10C8(struct G *p)
{
    if (!p->p45cc) {
        struct MSG msg;
        p->p45c8 = PloNew__F3CIDP2SO3OIDi(0x11, 0, 0x2CA, -1);
        func_001C0648(p->p45c8);
        msg.cb = 0x30;
        msg.nKind = 0x10;
        p->p45cc = func_001AE2A0(&msg);
    }
    func_001C1138(p->p45cc);
}
