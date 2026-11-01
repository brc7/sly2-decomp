// SLY2_ASM: gnu-reorder-db
// Static initialization (g++ __static_initialization_and_destruction_0) of the emitter defaults and six slot heaps.
// (Lilly's agent, T1656) MATCH, no register pins, no stand-ins.
//
// D_002DE5B0 / D_002DE5F0 are the EMITO / EMITV defaults that func_00159118 (InitEmitb) copies into EMITB+0x10 /
// +0x50 (Sly 1's InitEmitb builds the same two objects as guarded local statics). Their initializers hold the
// non-constant vectors D_002A2DD0 (0,0,0), D_002A2DA0 (1,0,0) and D_002A2DC0 (0,0,1), so g++ builds each one in a
// memset stack temp and block-copies it.
//
// The three unions are what matched it: store_constructor emits a (clobber (mem:BLK)) for every union initializer.
// - EMITV+0x60 is a CLQ (Sly 1 util.h: union of {g0, g1, g2, gUnused} and ag[4]) = {1, 0, -1}. Its clobber is a
//   scheduler barrier, so its two stores issue last and 1.0 / -1 die only there (the game's store order).
// - the union at EMITO+0x20 sits inside the live ranges of the first sentry %hi's only: those of D_002DD2E4 / E8 grow
//   to 150 / 168 insns against 146 / 164 for D_002DD2EC / F0, which breaks their global-alloc ties (s4/s3, s6/s5),
//   and it fixes the D_002A2DD0 / D_002A2DA0 v0/v1 choice.
// - the union at EMITV+0x00 fixes the FPU numbers of 1.0 / -1500, the 200 / loop-end registers and the s1/a2 order.
//
// The `if (++D_002DD2xx == 1)` guards are g++ sentries (decl2.c get_sentry): weak objects with constructors get a
// common int __sn<asm name> each, so the counters have no source names of their own.
typedef int qword __attribute__((mode(TI)));

// Sly 1 util.h CLQ: quadratic g0 + x*(g1 + x*g2).
union CLQ {
    struct { float g0, g1, g2, gUnused; };
    float ag[4];
};

// 16-byte vector views; the field names of these two are unknown.
union QWV { qword qw; float ag[4]; };
union G4V { struct { float g0, g4, g8, gC; }; float ag[4]; };

struct EMITO {
    int x0[4];
    int x10;
    int x14[3];
    QWV v20;
    qword v30;
};

struct EMITV {
    G4V g0;
    int x10, x14;
    float x18;
    int x1C;
    qword v20;
    int x30;
    int x34;
    float x38, x3C;
    int x40[8];
    CLQ clq60;
};

typedef char check_emito[(sizeof(EMITO) == 0x40) ? 1 : -1];
typedef char check_emitv[(sizeof(EMITV) == 0x70) ? 1 : -1];

extern qword D_002A2DD0, D_002A2DA0, D_002A2DC0;

EMITO D_002DE5B0 = { {0, 0, 0, 0}, -1, {0, 0, 0}, {D_002A2DD0}, D_002A2DA0 };
EMITV D_002DE5F0 = { {{750.0f, 750.0f, -1.0f, 1.0f}}, 0, 0, -1500.0f, 0, D_002A2DC0, 0, 200, 1.0f, 1.0f,
                     {0, 0, 0, 0, 0, 0, 0, 0}, {{1.0f, 0.0f, -1.0f}} };

// func_0011CCA8 returns its first argument: it is the SLOTHEAP constructor (cb, c, align).
struct SLOTHEAP {
    SLOTHEAP(int cb, int c, int align);
    int cb, c, align; void *head; int allocated; void *block;
};
extern SLOTHEAP D_004520A8 __attribute__((weak));
SLOTHEAP D_004520A8(0x14, 0x2000, 0x10);
extern SLOTHEAP D_004520D0 __attribute__((weak));
SLOTHEAP D_004520D0(0x18, 0x800, 0x10);
extern SLOTHEAP D_004520F8 __attribute__((weak));
SLOTHEAP D_004520F8(0x28, 0x200, 0x10);
extern SLOTHEAP D_00452120 __attribute__((weak));
SLOTHEAP D_00452120(0x1C, 0x800, 0x10);
extern SLOTHEAP D_00452148 __attribute__((weak));
SLOTHEAP D_00452148(0xC, 0xC8, 0x10);
extern SLOTHEAP D_00452170 __attribute__((weak));
SLOTHEAP D_00452170(0x14, 0x100, 0x10);

/** @sly2-annot addr=0x15CAD8
 * Purpose: compiler-generated __static_initialization_and_destruction_0 of TU emitter
 * Info: proposed name StaticInit_emitter (T1); TU emitter
 * Notes: real local symbol name is __static_initialization_and_destruction_0 (not unique; renamed per TU)
 * Confidence: high [T1 0.95] - evidence: static analysis; sources: tu_map(structural)+r3_1539A8
 * @end-sly2-annot */
__asm__(".text\n"
        ".globl func_0015CAD8\n"
        ".type func_0015CAD8,@function\n"
        "func_0015CAD8:\n"
        ".size func_0015CAD8,620\n");
