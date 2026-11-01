class M {
public:
    M() {}
    char pad[0x120 - 0x10];
    virtual void G();
};
class BASIC {
public:
    int m0; int m4; int m8;
    BASIC() { m0 = 0; m4 = 0; }
    virtual void F();
};
class X : public BASIC, public M {
public:
    char pad2[0x140 - 0x120 - 4];
    virtual void F();
};

extern "C" void *NewHnd(void)
{
    return new X;
}
