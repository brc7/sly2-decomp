extern char D_003DD490[];
extern char D_003DD040[];
void SetSndmgrMasterVolume(void *p, float f);
void WipeToWorldWarp(void *a, void *b, int c, int d, void *e, int f);
/** @sly2-annot addr=0x14C140
 * Purpose: 0x1FE9F8(D_003DD040, 0.0) then 0x22B490(g_sw, NULL, 0x189, -1, nParam, -1)
 * Info: proposed name CheatResetEpisode (T2); TU cheat
 * Confidence: low (draft) [T2 0.35] - evidence: semantic review; sources: semantic_tu_docs/cheat
 * See: docs/reference/functions/main-loop-startup.md#cheat-0x14bea8-0x14c548
 * @end-sly2-annot */
void func_0014C140(void *p)
{
    SetSndmgrMasterVolume(D_003DD040, 0.0f);
    WipeToWorldWarp(D_003DD490, 0, 0x189, -1, p, -1);
}
