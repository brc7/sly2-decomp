// NewBarrel MATCH (Claude agent, lane A12, 2026-10-23), from v3 (1): commented pin on the SUB vtable pointer (a0, as the target) plus a commented volatile empty asm before the vt store, which keeps `sw a0,0x170` ahead of the return move (sched2 tie-break).
extern "C" void func_00167310(void *);
class CRef {
public:
    CRef() { func_00167310(this); }
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
    char pad2[0x170 - 0x90];
    virtual void F();
};
extern "C" char D_002D2090[];
class SUB {
public:
    void *vt;
    SUB() { register void *p __asm__("$4") = D_002D2090; /* pin */ __asm__ volatile(""); /* scheduling barrier (toolchain emulation) */ vt = p; }
};
class Y : public X {
public:
    SUB sub;
    char padend[0x1a0 - 0x174];
    virtual void F();
};

/** @sly2-annot addr=0x112100
 * Purpose: pfnNew factory for CID88 (cid 88): allocates 0x1A0 bytes (operator new 0x1AE4F8), calls ctor
 *   __4CRef
 * Info: proposed name NewBarrel (T1); TU crt0_splicewrap
 * Notes: v3 class identity: NewCid88 -> NewBarrel
 * Confidence: high [T1 0.95] - evidence: static naming pass; sources: r0_thunks+table-verified(D_002BAAE8
 *   pfnNew/size 185/185; vt slot6 199/199)+class_review(master_names_v2_1)
 * See: docs/reference/functions/crt0-thunks.md#crt0_splicewrap-0x100000-0x1147d8
 * @end-sly2-annot */
extern "C" void *NewBarrel(void)
{
    return new Y;
}
