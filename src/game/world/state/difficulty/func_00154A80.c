/** @sly2-annot addr=0x154A80
 * Purpose: Clears the three difficulty sample references (p8/pC/p10)
 * Info: proposed name ClearDifficulty (T2); TU difficulty
 * Confidence: low (draft) [T2 0.35] - evidence: static analysis; sources: r3_1539A8
 * See: docs/reference/functions/game-progression.md#difficulty-0x154a60-0x1553f0
 * @end-sly2-annot */
void func_00154A80(char *p)
{
    *(int *)(p + 0x8) = 0;
    *(int *)(p + 0xC) = 0;
    *(int *)(p + 0x10) = 0;
}
