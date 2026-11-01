struct C { int a[4]; };
struct CRef { int m_tagk; int m_n; };
struct S { char pad[0x184]; int f184; };
extern char D_003D4A60[];
void func_001AE478(struct C *);
void func_001AE4A0(struct C *, int);
void *func_0013D1A8(struct S *);
void func_002581C0(struct CRef *, void *, void *);
void func_00129EC8(struct CRef *);
static inline int NFromRef(void *pv)
{
    struct CRef ref;
    int n;
    func_002581C0(&ref, pv, D_003D4A60);
    n = ref.m_n;
    func_00129EC8(&ref);
    return n;
}
/** @sly2-annot addr=0x13D248
 * Purpose: Read saved kind for this container (default +0x184); remap 0x2BF..0x2C1 -> 0x2BC..0x2BE, 7..9 ->
 *   -1
 * Info: proposed name NBootyContainerKind (T1); TU booty
 * Notes: verifier better_name (CORRECT row): Reads save-var via PkeyBootyContainerSave key; 0 -> default
 *   +0x184; 7..9 -> -1; 0x2BF-0x2C1 -> 0x2BC-0x2BE; sole caller SpawnBootyInContainer. Drop the trailing
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r2b_rest+verdicts_followups
 * See: docs/engine/game-progression.md#8-key-functions,
 *   docs/reference/functions/game-progression.md#booty-0x13bdf0-0x13d558
 * @end-sly2-annot */
int func_0013D248(struct S *p) {
    struct C ctx;
    int n;
    func_001AE478(&ctx);
    n = NFromRef(func_0013D1A8(p));
    if (n == 0) n = p->f184;
    else if (n >= 0) { if (n < 10) { if (n >= 7) n = -1; } }
    switch (n) {
    case 0x2bf: n = 0x2bc; break;
    case 0x2c0: n = 0x2bd; break;
    case 0x2c1: n = 0x2be; break;
    }
    func_001AE4A0(&ctx, 2);
    return n;
}
