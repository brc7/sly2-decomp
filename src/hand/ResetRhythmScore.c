/** @sly2-annot addr=0x153800
 * Purpose: Zero score count/average/smoothed (+0x23C,+0x284,+0x288)
 * Info: proposed name ResetRhythmScore (T1); TU rhythm
 * Confidence: high [T1 0.85] - evidence: semantic review; sources: r2_131F70+verdicts_tu_r1r2
 * See: docs/engine/gameplay-objects.md#key-functions,
 *   docs/reference/functions/gameplay-objects.md#rhythm-0x1520c8-0x1539a8
 * @end-sly2-annot */
void ResetRhythmScore(char *p)
{
    *(int *)(p + 0x23C) = 0;
    *(int *)(p + 0x284) = 0;
    *(int *)(p + 0x288) = 0;
}
