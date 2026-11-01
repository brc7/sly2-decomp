struct S {
    int x0, x4, x8, xC;
    struct S *x10;
    int x14;
    char pad[0x10];
    union { unsigned long u; struct { unsigned long f0 : 40; unsigned long b40 : 8; unsigned long f48 : 4; long st : 4; unsigned long f56 : 4; unsigned long f60 : 1; unsigned long f61 : 3; } b; } x28;
    int x30, x34;
    float x38;
    int x3C;
    int x40;
};
extern int D_003E139C;
void func_001A53A0(struct S *s);
void func_001A5448(struct S *p, void **a, void ***b);
void func_0011A280(void *a, void *b, struct S *s);
/** @sly2-annot addr=0x1A5270
 * Purpose: LO vt7 Init: busy fields (nibble48=0xF, busyk=2, byte40=0xFF, sBusy x38=1e10), inherit
 *   loroot+in-world bit60 from parent or EnsureLoroot, grf x40=0x21, ppxr x3C from parent else D_003E139C,
 *   prepend to parent child DL (in-world region)
 * Info: proposed name InitLo (T1); class LO slot7; TU lo
 * Notes: runtime: LO +0x28 nibble48/busyk/bit60, +0x38 sBusy 1e10, +0x3C ppxr, +0x40 grf 0x21 confirmed
 * Confidence: high [T1 0.95] - evidence: runtime (save-state RAM); sources:
 *   r4_17F090+verdicts_tu_r4r6+runtime
 * See: docs/engine/animation.md#7-key-functions, docs/engine/object-model.md#2-the-lo-virtual-interface,
 *   docs/engine/object-model.md#8-source-files
 * @end-sly2-annot */
void func_001A5270(struct S *s)
{
    void *a;
    void **b;
    s->x28.b.f48 = 0xF;
    s->x38 = 1e10f;
    s->x28.b.st = 2;
    s->x28.b.b40 = 0xFF;
    if (s->x10) {
        s->x14 = s->x10->x14;
        s->x28.b.f60 = (int)(s->x10->x28.u >> 60);
    } else {
        func_001A53A0(s);
        s->x28.b.f60 = 1;
    }
    s->x40 = 0x21;
    s->x3C = s->x10 ? s->x10->x3C : D_003E139C;
    func_001A5448(s->x10, &a, &b);
    func_0011A280(a, *b, s);
}
