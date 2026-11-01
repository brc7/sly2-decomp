int func_00161FC8(int i);
/** @sly2-annot addr=0x161FF0
 * Purpose: ORs GrfFromState over an index array
 * Info: proposed name GrfFromStates (T1); TU game
 * Confidence: mid (draft) [T1 0.55] - evidence: static analysis; sources: r3b_rest
 * See: docs/reference/functions/game-progression.md#game-0x1613a8-0x166270
 * @end-sly2-annot */
int func_00161FF0(int c, int *ai)
{
    int grf = 0;
    int i;
    for (i = 0; i < c; i++)
        grf |= func_00161FC8(ai[i]);
    return grf;
}
