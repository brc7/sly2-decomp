void SetSndmgrMasterVolume(void *, float);
void SetSndmgrChannelVolume(void *, int, int, float);
/** @sly2-annot addr=0x1FEDF0
 * Purpose: Master=1 and all weights=1 (except category 1)
 * Info: proposed name ResetSndmgrVolumes (T1); TU sound
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: r7_1F8DD8
 * See: docs/reference/functions/sound.md#sound-0x1faf70-0x1ff3b8
 * @end-sly2-annot */
void ResetSndmgrVolumes(void *p)
{
    int i, j;
    SetSndmgrMasterVolume(p, 1.0f);
    for (i = 0; i < 8; i++) {
        if (i == 1) continue;
        for (j = 0; j < 4; j++) SetSndmgrChannelVolume(p, j, i, 1.0f);
    }
}
