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
    char pad[0x30 - 0x10];
    M m;
    virtual void F();
};

/** @sly2-annot addr=0x113840
 * Purpose: pfnNew factory for BHV191 (cid 191): allocates 0x50 bytes (operator new 0x1AE4F8), calls ctor
 *   func_002562A8
 * Info: proposed name NewBhv191 (T1); TU crt0_splicewrap
 * Confidence: high [T1 0.95] - evidence: static analysis; sources: r0_thunks+table-verified(D_002BAAE8
 *   pfnNew/size 185/185; vt slot6 199/199)
 * See: docs/reference/functions/crt0-thunks.md#crt0_splicewrap-0x100000-0x1147d8
 * @end-sly2-annot */
extern "C" void *NewBhv191(void)
{
    return new X;
}
