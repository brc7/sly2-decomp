struct CTX { int a[4]; CTX(); ~CTX(); };
struct AREQ { int cb; int align; AREQ(int c, int a) { cb = c; align = a; } };
struct I { int a; void *b; };
struct E { int id; int n; int c; I *a; };
struct P {
    char pad[0x150];
    int c150;
    void *x154;
    int pad158;
    int i;
    int c;
    E *ae;
};
extern "C" {
void *func_001AE318(const AREQ &r);
void *func_001AE378(const AREQ &r);
void *func_001AE238(const AREQ &r, void *pv);
}
static inline void *Alloc(int cb) { return func_001AE318(AREQ(cb, 0x10)); }
static inline void *AllocB(int cb) { return func_001AE378(AREQ(cb, 0x10)); }
static inline void *Realloc(void *pv, int cb) { return func_001AE238(AREQ(cb, 0x10), pv); }
extern "C" {
void func_001F31E0(P *p, void *a);
}

/** @sly2-annot addr=0x1BABA0
 * Purpose: CID 98 vt14: allocate trigger-class list and stage table (with stage -1), LoadSoFromBrx, trim
 *   allocations to counts
 * Info: proposed name LoadCid98FromBrx (T2); class CID98 slot14; TU tu_1BAB50
 * Notes: level_parser CONFIRMED-parse: payload layout followed by the parser, 100%% of bytes in 4 levels
 * Confidence: low (draft) [T2 0.75] - evidence: static analysis; sources: r5b_rest+parse-verified
 * See: docs/engine/gameplay-objects.md#key-functions,
 *   docs/formats/level-objects.md#5-per-class-payloads-vtable-slot-14,
 *   docs/reference/functions/gameplay-objects.md#tu_1bab50-0x1bab50-0x1bc050
 * @end-sly2-annot */
extern "C" void func_001BABA0(P *p, void *a)
{
    CTX ctx;
    int i, j;

    p->x154 = Alloc(0x40);
    p->ae = (E *)AllocB(0x200) + 1;
    func_001F31E0(p, a);
    p->i = 0;
    p->x154 = Realloc(p->x154, p->c150 << 3);
    if (p->c) {
        p->ae = (E *)Realloc(p->ae - 1, (p->c << 4) + 0x10) + 1;
        for (i = -1; i < p->c; i++) {
            E *pe = &p->ae[i];
            pe->a = (I *)Realloc(pe->a, pe->c << 3);
            for (j = 0; j < pe->c; j++) {
                I *pi = &pe->a[j];
                if (pi->a == 0xC) {
                    pi->b = Realloc(pi->b, 8);
                }
            }
        }
    } else {
        p->ae = 0;
    }
}
