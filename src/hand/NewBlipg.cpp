class E {
public:
    E() {}
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
    char pad[0x60 - 0x10];
    E ae[32];
    virtual void F();
};

extern "C" void *NewBlipg(void)
{
    return new X;
}
