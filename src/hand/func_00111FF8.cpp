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
    char padend[0x1d0 - 0x150];
    virtual void F();
};

/** @sly2-annot addr=0x111FF8
 * Purpose: pfnNew factory for CID85 (cid 85): allocates 0x1D0 bytes (operator new 0x1AE4F8), calls ctor
 *   __4CRef
 * Info: proposed name NewLasen (T1); TU crt0_splicewrap
 * Notes: v3 class identity: NewCid85 -> NewLasen
 * Confidence: high [T1 0.95] - evidence: static naming pass; sources: r0_thunks+table-verified(D_002BAAE8
 *   pfnNew/size 185/185; vt slot6 199/199)+class_review(master_names_v2_1)
 * See: docs/reference/functions/crt0-thunks.md#crt0_splicewrap-0x100000-0x1147d8
 * @end-sly2-annot */
extern "C" void *func_00111FF8(void)
{
    return new Y;
}
