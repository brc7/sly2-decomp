typedef struct CRef { int m_tagk; int m_tag; } CRef;
extern int D_003E0FB0;
extern char D_003D4A60[];
void func_002581C0(CRef *pref, int a, void *b);
void func_00129EC8(CRef *pref);
/** @sly2-annot addr=0x161798
 * Purpose: Reads the int save var at g_sw+0x3B20 (a collectible counter)
 * Info: proposed name CPrize91Get (T2); TU game
 * Confidence: low (draft) [T2 0.35] - evidence: static analysis; sources: r3b_rest
 * See: docs/reference/functions/game-progression.md#game-0x1613a8-0x166270,
 *   docs/reference/functions/game-progression.md#prize-0x1c4e50-0x1c8cc0
 * @end-sly2-annot */
int func_00161798(void)
{
    CRef ref;
    int n;
    func_002581C0(&ref, D_003E0FB0, D_003D4A60);
    n = ref.m_tag;
    func_00129EC8(&ref);
    return n;
}
