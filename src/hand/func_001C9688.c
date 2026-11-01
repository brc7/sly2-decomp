struct RGBA { unsigned char r, g, b, a; };
struct E { int n; void *p; struct RGBA c; };
extern struct E D_003D3A00[5];
extern char D_002C1740[], D_002C19C0[], D_002C1AC8[], D_002C1B50[], D_002C1BD8[];
extern int D_002DD2E0, D_002DD2E4, D_002DD2E8, D_002DD2EC, D_002DD2F0, D_002DD2F4;
extern char D_004520A8[], D_004520D0[], D_004520F8[], D_00452120[], D_00452148[], D_00452170[];
void *func_002333B4(void *p, int c, int cb);
void func_0011CCA8(void *p, int cb, int c, int align);
static inline void SetRgba(struct RGBA *pc, int r, int g, int b, int a) { pc->r = r; pc->g = g; pc->b = b; pc->a = a; }
static inline void InitE(struct E *pe, int n, void *p, int r, int g, int b, int a)
{
    func_002333B4(pe, 0, sizeof(struct E));
    pe->n = n;
    pe->p = p;
    SetRgba(&pe->c, r, g, b, a);
}
/** @sly2-annot addr=0x1C9688
 * Purpose: compiler-generated __static_initialization_and_destruction_0 of TU prog
 * Info: proposed name StaticInit_prog (T1); TU prog
 * Notes: real local symbol name is __static_initialization_and_destruction_0 (not unique; renamed per TU)
 * Confidence: high [T1 0.95] - evidence: static analysis; sources: tu_map(structural)+r5_1AE130
 * @end-sly2-annot */
void func_001C9688(int init, int prio)
{
    if (prio == 0xFFFF && init) {
        struct E *pe = D_003D3A00;
        func_002333B4(pe, 0, sizeof(struct E));
        pe->n = 0x35;
        pe->p = D_002C1740;
        SetRgba(&pe->c, 0x2F, 0x3A, 0x7F, 0x7F);
        pe++;
        func_002333B4(pe, 0, sizeof(struct E));
        pe->n = 0x16;
        pe->p = D_002C19C0;
        SetRgba(&pe->c, 0x2F, 0x3A, 0x7F, 0x7F);
        pe++;
        func_002333B4(pe, 0, sizeof(struct E));
        pe->n = 0xB;
        pe->p = D_002C1AC8;
        SetRgba(&pe->c, 0x5F, 0x5F, 0x5F, 0x7F);
        pe++;
        func_002333B4(pe, 0, sizeof(struct E));
        pe->n = 0xB;
        pe->p = D_002C1B50;
        SetRgba(&pe->c, 0x5F, 0x5F, 0x5F, 0x7F);
        pe++;
        func_002333B4(pe, 0, sizeof(struct E));
        pe->n = 0x3E;
        pe->p = D_002C1BD8;
        SetRgba(&pe->c, 0x5F, 0x5F, 0x5F, 0x7F);
        if (++D_002DD2E0 == 1) func_0011CCA8(D_004520A8, 0x14, 0x2000, 0x10);
        if (++D_002DD2E4 == 1) func_0011CCA8(D_004520D0, 0x18, 0x800, 0x10);
        if (++D_002DD2E8 == 1) func_0011CCA8(D_004520F8, 0x28, 0x200, 0x10);
        if (++D_002DD2EC == 1) func_0011CCA8(D_00452120, 0x1C, 0x800, 0x10);
        if (++D_002DD2F0 == 1) func_0011CCA8(D_00452148, 0xC, 0xC8, 0x10);
        if (++D_002DD2F4 == 1) func_0011CCA8(D_00452170, 0x14, 0x100, 0x10);
    }
}
