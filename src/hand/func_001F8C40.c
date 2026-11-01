typedef int qword __attribute__((mode(TI)));
struct S { qword q0; int n10; int pad[3]; };
extern struct S D_003DD020;
extern qword D_002A2DC0;
extern int D_002DD2E0, D_002DD2E4, D_002DD2E8, D_002DD2EC, D_002DD2F0, D_002DD2F4;
extern char D_004520A8[], D_004520D0[], D_004520F8[], D_00452120[], D_00452148[], D_00452170[];
void *func_002333B4(void *, int, int);
void func_0011CCA8(void *, int, int, int);
/** @sly2-annot addr=0x1F8C40
 * Purpose: compiler-generated __static_initialization_and_destruction_0 of TU so
 * Info: proposed name StaticInit_so (T1); TU so
 * Notes: real local symbol name is __static_initialization_and_destruction_0 (not unique; renamed per TU)
 * Confidence: high [T1 0.95] - evidence: static analysis; sources: tu_map(structural)+r6_1D5150
 * See: docs/engine/physics-collision.md#1-so-the-solid-object-socpp-0x1f2108-0x1f8dd8-174-functions
 * @end-sly2-annot */
void func_001F8C40(int init, int prio)
{
    if (prio == 0xFFFF && init) {
        struct S *pg = &D_003DD020;
        struct S t;
        func_002333B4(&t, 0, 0x20);
        t.q0 = D_002A2DC0;
        t.n10 = 1;
        *pg = t;
        if (++D_002DD2E0 == 1) func_0011CCA8(D_004520A8, 0x14, 0x2000, 0x10);
        if (++D_002DD2E4 == 1) func_0011CCA8(D_004520D0, 0x18, 0x800, 0x10);
        if (++D_002DD2E8 == 1) func_0011CCA8(D_004520F8, 0x28, 0x200, 0x10);
        if (++D_002DD2EC == 1) func_0011CCA8(D_00452120, 0x1C, 0x800, 0x10);
        if (++D_002DD2F0 == 1) func_0011CCA8(D_00452148, 0xC, 0xC8, 0x10);
        if (++D_002DD2F4 == 1) func_0011CCA8(D_00452170, 0x14, 0x100, 0x10);
    }
}
