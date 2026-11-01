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

/** @sly2-annot addr=0x110EC0
 * Purpose: pfnNew factory for CID19 (cid 19): allocates 0x140 bytes (operator new 0x1AE4F8), inline ctor
 *   stores vtable 0x2D2978 at +0xC
 * Info: proposed name NewHnd (T1); TU crt0_splicewrap
 * Notes: v3 class identity: NewCid19 -> NewHnd
 * Confidence: high [T1 0.95] - evidence: static naming pass; sources: r0_thunks+table-verified(D_002BAAE8
 *   pfnNew/size 185/185; vt slot6 199/199)+class_review(master_names_v2_1)
 * See: docs/reference/functions/crt0-thunks.md#crt0_splicewrap-0x100000-0x1147d8
 * @end-sly2-annot */
extern "C" void *NewHnd(void)
{
    return new X;
}
