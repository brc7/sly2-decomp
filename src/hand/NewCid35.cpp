class M {
public:
    M() {}
    char pad[0x100 - 0x10];
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
    char pad2[0x180 - 0x100 - 4];
    virtual void F();
};

/** @sly2-annot addr=0x111260
 * Purpose: pfnNew factory for CID35 (cid 35): allocates 0x180 bytes (operator new 0x1AE4F8), inline ctor
 *   stores vtable 0x2D1EE8 at +0xC
 * Info: proposed name NewCid35 (T1); TU crt0_splicewrap
 * Confidence: high [T1 0.95] - evidence: static analysis; sources: r0_thunks+table-verified(D_002BAAE8
 *   pfnNew/size 185/185; vt slot6 199/199)
 * See: docs/reference/functions/crt0-thunks.md#crt0_splicewrap-0x100000-0x1147d8
 * @end-sly2-annot */
extern "C" void *NewCid35(void)
{
    return new X;
}
