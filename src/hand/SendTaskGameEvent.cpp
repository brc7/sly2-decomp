class O {
public:
    int a, b, c;
    virtual void V1();
    virtual void V2();
    virtual void Msg(int msgid, int a, int c, void **apv, int d);
};
extern "C" int FTaskLive(O *p);
extern "C" void SendTaskGameEvent(O *p, int n, int m)
{
    if (FTaskLive(p)) {
        void *apv[2];
        apv[0] = &n;
        apv[1] = &m;
        p->Msg(0x11, 0, 2, apv, 0);
    }
}
