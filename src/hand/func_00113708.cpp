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
    char pad[0x40 - 0x10];
    M m0;
    M m1;
    M m2;
    char pad2[0xD0 - 0xA0];
    virtual void F();
};

/** @sly2-annot addr=0x113708
 * Purpose: pfnNew factory for BHV186 (cid 186): allocates 0xD0 bytes (operator new 0x1AE4F8), calls ctor
 *   func_002562A8, func_002562A8, func_002562A8
 * Info: proposed name NewBhvgoto (T1); TU crt0_splicewrap
 * Notes: BHV class word normalised: NewBhv186 -> NewBhvgoto | runtime2/4 confirmed the BHVGOTO role on live
 *   guards (S02-S41)
 * Confidence: high [T1 0.97] - evidence: runtime (save-state RAM); sources:
 *   r0_thunks+table-verified(D_002BAAE8 pfnNew/size 185/185; vt slot6 199/199)+runtime(class role)
 * See: docs/reference/functions/crt0-thunks.md#crt0_splicewrap-0x100000-0x1147d8
 * @end-sly2-annot */
extern "C" void *func_00113708(void)
{
    return new X;
}
