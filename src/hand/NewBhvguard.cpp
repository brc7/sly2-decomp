extern "C" void *func_002333B4(void *pv, int b, int cb);
class M {
public:
    M() { func_002333B4(this, 0, sizeof(*this)); }
    int a[0x70 / 4];
};
class BASIC {
public:
    int m0; int m4; int m8;
    BASIC() { m0 = 0; m4 = 0; }
    virtual void F();
};
class X : public BASIC {
public:
    char pad[0x30 - 0x10];
    M m;
    char pad2[0xB0 - 0xA0];
    virtual void F();
};

extern "C" void *NewBhvguard(void)
{
    return new X;
}
