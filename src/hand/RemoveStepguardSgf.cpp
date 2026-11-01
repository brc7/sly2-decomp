class OBase { public: int a, b, c; virtual void V1(); virtual void V2(); virtual void Msg(int msg, int a, int c, void **apv, int x); };
class O : public OBase { public: char pad[0x2C0 - 0x10]; unsigned long fl; };
extern "C" unsigned long GrfFromSgf(int n);
extern "C" void SetStepguardGrfsgf(O *p, unsigned long fl);
extern "C" void RemoveStepguardSgf(O *p, int n)
{
    int m;
    void *apv[2];
    SetStepguardGrfsgf(p, p->fl & ~GrfFromSgf(n));
    m = -1;
    apv[0] = &m;
    apv[1] = &n;
    p->Msg(7, 0, 2, apv, 0);
}
