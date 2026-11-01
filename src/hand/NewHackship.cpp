class CRef {
public:
    CRef();
    int a[4];
};
class BASIC {
public:
    int m0; int m4; int m8;
    BASIC() { m0 = 0; m4 = 0; }
    virtual void F();
};
class X : public BASIC {
public:
    char pad[0x80 - 0x10];
    CRef ref;
    char pad2[0x150 - 0x90];
    virtual void F();
};
class Y : public X {
public:
    char pad_174[0x174 - 0x150];
    int n174;
    char pad_2a4[0x2a4 - 0x178];
    int n2a4;
    char padend[0x2b0 - 0x2a8];
    Y() { n174 = 0; n2a4 = 0; }
    virtual void F();
};

extern "C" void *NewHackship(void)
{
    return new Y;
}
