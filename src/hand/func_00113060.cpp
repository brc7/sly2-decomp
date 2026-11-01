extern "C" void *func_002333B4(void *pv, int b, int cb);
class M {
public:
    M() { func_002333B4(this, 0, sizeof(*this)); }
    int a[0x70 / 4];
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
    char pad2[0xB0 - 0xA0];
    virtual void F();
};

/** @sly2-annot addr=0x113060
 * Purpose: pfnNew factory for BHV156 (cid 156): allocates 0xB0 bytes (operator new 0x1AE4F8), calls ctor
 *   memset
 * Info: proposed name NewBhvguard (T1); TU crt0_splicewrap
 * Notes: BHV class word normalised: NewBhv156 -> NewBhvguard | runtime2/4 confirmed the BHVGUARD role on live
 *   guards (S02-S41)
 * Confidence: high [T1 0.97] - evidence: runtime (save-state RAM); sources:
 *   r0_thunks+table-verified(D_002BAAE8 pfnNew/size 185/185; vt slot6 199/199)+runtime(class role)
 * See: docs/reference/functions/crt0-thunks.md#crt0_splicewrap-0x100000-0x1147d8
 * @end-sly2-annot */
extern "C" void *func_00113060(void)
{
    return new X;
}
