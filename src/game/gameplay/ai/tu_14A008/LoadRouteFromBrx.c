struct S { int a, b; };
struct F { int a[4]; };
struct P { char pad[0x54]; int n54; int n58; };
struct F *STACKSCOPE_ctor(struct F *f);
int func_001AE378(struct S *s);
void LoadLoFromBrx(struct P *p, int a);
int func_001AE238(struct S *s, int n);
void STACKSCOPE_ctor_1AE4A0(struct F *f, int n);
static inline void InitS(struct S *ps, int a, int b)
{
    ps->a = a * 0xC;
    ps->b = b;
}
/** @sly2-annot addr=0x14A008
 * Purpose: cid 0x7B (LO, 0x60) vt14: stack-alloc a 256-entry point table, LoadLoFromBrx (Splice options fill
 *   it via 0x14A178), copy used part to the heap
 * Info: proposed name LoadRouteFromBrx (T1); class CID123 slot14; TU tu_14A008
 * Notes: level_parser CONFIRMED-code (layout read from the matched function; class absent from samples) |
 *   whole_game: loader read in all 43 levels (base reader only, nothing extra; CONFIRMED code+parse)
 * Confidence: mid (draft) [T1 0.75] - evidence: static analysis; sources: r2b_rest+parse-code+parse-verified
 * See: docs/engine/guards-ai.md#8-source-files, docs/engine/guards-ai.md#9-key-functions,
 *   docs/engine/world-sw.md#9-source-files
 * @end-sly2-annot */
void LoadRouteFromBrx(struct P *p, int a)
{
    struct F f;
    struct S s;
    STACKSCOPE_ctor(&f);
    InitS(&s, 0x100, 0x10);
    p->n58 = func_001AE378(&s);
    LoadLoFromBrx(p, a);
    InitS(&s, p->n54, 0x10);
    p->n58 = func_001AE238(&s, p->n58);
    STACKSCOPE_ctor_1AE4A0(&f, 2);
}
