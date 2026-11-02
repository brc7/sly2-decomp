typedef struct CRef { int m_tagk; int m_tag; } CRef;
void func_002013D8(CRef *pref, void *p, int n, int c, int *pc, int *ai);
int GrfFromStates(int c, int *ai);
void Decref__4CRef(CRef *pref);
/** @sly2-annot addr=0x162050
 * Purpose: Converts a Splice list argument (otyp 0x1050, max 6) of state numbers into a state bit mask
 * Info: proposed name GrfStatesFromSplice (T2); TU game
 * Confidence: low (draft) [T2 0.45] - evidence: static analysis; sources: r3b_rest
 * See: docs/reference/functions/game-progression.md#game-0x1613a8-0x166270
 * @end-sly2-annot */
int func_00162050(void *p)
{
    int ai[8];
    CRef ref;
    int c;
    int grf;
    func_002013D8(&ref, p, 0x1050, 6, &c, ai);
    grf = GrfFromStates(c, ai);
    Decref__4CRef(&ref);
    return grf;
}
