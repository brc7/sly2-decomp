class O {
public:
    int a, b, c;
    virtual void V1();
    virtual void V2();
    virtual void Msg(int msgid, int a, int c, void **apv, int d);
};
extern "C" int FTaskLive(O *p);
extern "C" void func_00165118(O *p, int n)
{
    if (FTaskLive(p)) {
        void *apv[1];
        apv[0] = &n;
        p->Msg(0x17, 0, 1, apv, 0);
    }
}
