/** @sly2-annot addr=0x161FC8
 * Purpose: 1<<state, 0 for -1
 * Info: proposed name GrfFromState (T1); TU game
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r3b_rest+verdicts_followups
 * See: docs/engine/game-progression.md#8-key-functions,
 *   docs/reference/functions/game-progression.md#game-0x1613a8-0x166270
 * @end-sly2-annot */
int func_00161FC8(int i)
{
    if (i == -1) return 0;
    return (int)(1L << i);
}
