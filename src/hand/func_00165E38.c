typedef struct CRef { int m_tagk; int m_tag; } CRef;
struct G { char pad[0x3B1C]; void *f3b1c; char pad2[0x3B28 - 0x3B20]; void *f3b28; char pad3[0x3B7C - 0x3B2C]; void *a3b7c[1]; };
struct Q { int a[4]; int f10; int f14; int f18; };
extern struct G D_003DD490;
extern struct Q D_002DEB30;
extern void *D_002DEB4C;
extern char D_003DA530[];
extern char D_002A3828[];
CRef *func_002581C0(CRef *pref, void *a, void *b);
void func_00129EC8(CRef *pref);
int func_001D9880(void *p);
void func_001D54B8(void);
void func_001DD888(void *p);
void *PrecWorldEntry(int n);
void func_0022B490(struct G *pg, void *p, int a, int b, int c, int d);
/** @sly2-annot addr=0x165E38
 * Purpose: Starts play from a save-slot GS: new game -> first task oid 0x62F in world entry 0; else the saved
 *   episode (or hub level D_002A3828 if it is 100%)
 * Info: proposed name PlayGameFromGs (T2); TU game
 * Confidence: low (draft) [T2 0.45] - evidence: static analysis; sources: r3b_rest
 * See: docs/formats/disc-records.md#3-static-records-in-the-elf,
 *   docs/reference/functions/game-progression.md#game-0x1613a8-0x166270,
 *   docs/reference/functions/save-memcard.md#memcard-0x1aa978-0x1ae150
 * @end-sly2-annot */
void func_00165E38(void *pv)
{
    CRef ref;
    float g;
    register int n __asm__("$16"); /* register pin: n lives in s0 in the target */
    void *ps;
    void *prop;
    func_002581C0(&ref, D_003DD490.f3b1c, pv);
    g = *(float *)&ref.m_tag;
    func_00129EC8(&ref);
    if (func_001D9880(D_003DA530) && g == 0.0f) {
        func_001D54B8();
        func_001DD888(D_003DA530);
        return;
    }
    if (g == 0.0f) {
        D_002DEB4C = pv;
        D_002DEB30.f10 = -1;
        D_002DEB30.f14 = -1;
        D_002DEB30.f18 = 0x62F;
        func_0022B490(&D_003DD490, PrecWorldEntry(0), 0x189, -1, 0x48, -1);
        return;
    }
    {
        struct G *pg = &D_003DD490;
        D_002DEB4C = pv;
        D_002DEB30.f10 = -1;
        D_002DEB30.f14 = -1;
        D_002DEB30.f18 = -1;
        func_002581C0(&ref, pg->f3b28, pv);
    }
    __asm__ __volatile__(""); /* ordering-only: ends the -1 constant's dependence chain at the call */
    n = ref.m_tag;
    func_00129EC8(&ref);
    ps = PrecWorldEntry(n);
    prop = D_003DD490.a3b7c[n];
    n = 0;
    if (prop) {
        func_002581C0(&ref, prop, pv);
        n = ref.m_tag;
        func_00129EC8(&ref);
    }
    if (n >= 100 || !ps) {
        ps = D_002A3828;
        __asm__ __volatile__("" : : "r"(n)); /* allocation-only: keeps n live past the compare */
    }
    func_0022B490(&D_003DD490, ps, 0x189, -1, 0x40, -1);
}
