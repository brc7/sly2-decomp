extern int D_003E1C54;
extern int D_003E1C30;
struct L { int a[4]; };
struct M { int a; int b; int c[2]; };
void Align__18CBinaryInputStreami(void *q, int n);
void STACKSCOPE_ctor(struct L *pl);
void *func_001AE318(struct M *pm);
void Read__18CBinaryInputStreamiPv(void *q, int a, void *x);
void RelocateSharedColorTables(void *x);
void UploadBitmaps__FP3GSB(void *p);
void STACKSCOPE_ctor_1AE4A0(struct L *pl, int n);
void Skip__18CBinaryInputStreami(void *q, int a);
void func_00170828(void *p);
/** @sly2-annot addr=0x20F640
 * Purpose: If shared block changed: read cb bytes into slot alloc and rebuild (0x1EA568, 0x170730); else skip
 *   it and reuse (0x170828)
 * Info: proposed name LoadSwSharedDataFromBrx (T1); TU sw
 * Notes: level_parser CONFIRMED-parse: payload layout followed by the parser, 100%% of bytes in 4 levels
 * Confidence: mid (draft) [T1 0.75] - evidence: static analysis; sources: r7_1F8DD8+parse-verified
 * See: docs/engine/world-sw.md#10-key-functions,
 *   docs/formats/level-objects.md#2-per-world-entry-header-loop-body-of-loadswfrombrx,
 *   docs/formats/textures.md#1-layout-inside-a-world-entry-cstatic--cparse
 * @end-sly2-annot */
void LoadSwSharedDataFromBrx(void *p, void *q)
{
    Align__18CBinaryInputStreami(q, 0x10);
    if (D_003E1C54) {
        struct L l;
        struct M m;
        void *x;
        STACKSCOPE_ctor(&l);
        { struct M *pm = &m;
        pm->a = D_003E1C30;
        pm->b = 0x10;
        x = func_001AE318(pm); }
        Read__18CBinaryInputStreamiPv(q, D_003E1C30, x);
        RelocateSharedColorTables(x);
        UploadBitmaps__FP3GSB(p);
        STACKSCOPE_ctor_1AE4A0(&l, 2);
    } else {
        Skip__18CBinaryInputStreami(q, D_003E1C30);
        func_00170828(p);
    }
}
