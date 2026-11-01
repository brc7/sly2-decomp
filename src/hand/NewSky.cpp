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
    char padend[0x160 - 0x150];
    virtual void F();
};

/** @sly2-annot addr=0x1123C8
 * Purpose: pfnNew factory for CID97 (cid 97): allocates 0x160 bytes (operator new 0x1AE4F8), calls ctor
 *   __4CRef
 * Info: proposed name NewSky (T1); TU crt0_splicewrap
 * Notes: class identity from whole_game WE names/object strings: NewCid97 -> NewSky
 * Confidence: high [T1 0.95] - evidence: static analysis; sources: r0_thunks+table-verified(D_002BAAE8
 *   pfnNew/size 185/185; vt slot6 199/199)+whole_game(class identity)
 * See: docs/reference/functions/crt0-thunks.md#crt0_splicewrap-0x100000-0x1147d8
 * @end-sly2-annot */
extern "C" void *NewSky(void)
{
    return new Y;
}
