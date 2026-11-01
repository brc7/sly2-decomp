class M {
public:
    M();
    int a[8];
};
class BASIC {
public:
    int m0; int m4; int m8;
    BASIC() { m0 = 0; m4 = 0; }
    virtual void F();
};
class X : public BASIC {
public:
    char pad[0x40 - 0x10];
    M m0;
    M m1;
    M m2;
    char pad2[0xD0 - 0xA0];
    virtual void F();
};

extern "C" void *NewBhvgoto(void)
{
    return new X;
}
