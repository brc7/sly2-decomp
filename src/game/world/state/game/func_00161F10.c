struct E { int a; int b; };
/** @sly2-annot addr=0x161F10
 * Purpose: Appends (a,b) to a bounded pair array if *pc<cMax; first arg unused (callers pass &g_game)
 * Info: proposed name FAddPair (T2); TU game
 * Confidence: low (draft) [T2 0.45] - evidence: static analysis; sources: r3b_rest
 * See: docs/reference/functions/game-progression.md#game-0x1613a8-0x166270,
 *   docs/reference/functions/gameplay-objects.md#button-0x148620-0x149a50,
 *   docs/reference/functions/world-sw.md#exit_warp-0x227fe8-0x228b48
 * @end-sly2-annot */
int func_00161F10(int x, struct E *ae, int cMax, int *pc, int a, int b)
{
    struct E *pe;
    if (*pc < cMax) {
        pe = &ae[(*pc)++];
        pe->a = a;
        pe->b = b;
        return 1;
    }
    return 0;
}
