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

/** @sly2-annot addr=0x1110D8
 * Purpose: pfnNew factory for BLIPG (cid 28): allocates 0x460 bytes (operator new 0x1AE4F8), inline ctor
 *   stores vtable 0x2CB5D8 at +0xC
 * Info: proposed name NewBlipg (T1); TU crt0_splicewrap
 * Confidence: high [T1 0.95] - evidence: static analysis; sources: r0_thunks+table-verified(D_002BAAE8
 *   pfnNew/size 185/185; vt slot6 199/199)
 * See: docs/reference/functions/crt0-thunks.md#crt0_splicewrap-0x100000-0x1147d8
 * @end-sly2-annot */
extern "C" void *func_001110D8(void)
{
    return new X;
}
