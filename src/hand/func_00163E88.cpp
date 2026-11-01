class OBase {
public:
    int a, b, c;
    virtual void V1();
    virtual void V2();
    virtual void Msg(int msgid, int a, int c, void **apv, int d);
};
class O : public OBase {
public:
    char pad[0x7C - 0x10];
    O *f7c;
};
struct Q { int f0, f4, f8; };
extern Q D_002DEB30;
extern "C" int FTaskLive(O *p);
extern "C" int FJobLive(O *p);
extern "C" void BroadcastGameEvent(void *pv, int n, O *p);
extern "C" void func_00163E88(O *p, int n)
{
    void *apv[1];
    apv[0] = &n;
    if (FTaskLive(p)) {
        D_002DEB30.f8 = n;
        BroadcastGameEvent(&D_002DEB30, 0x27, p);
        p->Msg(0x12, 0, 1, apv, 0);
    }
    if (p->f7c && FJobLive(p->f7c))
        p->f7c->Msg(0x12, 0, 1, apv, 0);
}
