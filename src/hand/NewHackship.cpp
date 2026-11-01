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

/** @sly2-annot addr=0x111BC8
 * Purpose: pfnNew factory for CID72 (cid 72): allocates 0x2B0 bytes (operator new 0x1AE4F8), calls ctor
 *   __4CRef
 * Info: proposed name NewHackship (T1); TU crt0_splicewrap
 * Notes: class identity from whole_game WE names/object strings: NewCid72 -> NewHackship
 * Confidence: high [T1 0.95] - evidence: static analysis; sources: r0_thunks+table-verified(D_002BAAE8
 *   pfnNew/size 185/185; vt slot6 199/199)+whole_game(class identity)
 * See: docs/reference/functions/crt0-thunks.md#crt0_splicewrap-0x100000-0x1147d8
 * @end-sly2-annot */
extern "C" void *NewHackship(void)
{
    return new Y;
}
