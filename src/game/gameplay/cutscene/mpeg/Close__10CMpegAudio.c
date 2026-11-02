struct S { int a; char pad[0x9C]; int xa0; int xa4; };
void snd_CloseMovieSound(void);
void snd_SetMasterVolume(int n, int v);
void snd_ContinueAllSoundsInGroup(unsigned int m);
/** @sly2-annot addr=0x1B3190
 * Purpose: release IOP resources
 * Info: proposed name CMpegAudio_Close (T1); class CMpegAudio; TU mpeg
 * Confidence: high [T1 0.80] - evidence: static analysis; sources: r5_1AE130+verdicts_A2
 * See: docs/engine/cutscenes.md#5-key-functions, docs/reference/functions/cutscenes.md#mpeg-0x1b2820-0x1b4438
 * @end-sly2-annot */
void Close__10CMpegAudio(struct S *p)
{
    p->a = 0;
    snd_CloseMovieSound();
    snd_SetMasterVolume(2, p->xa0);
    snd_SetMasterVolume(0x10, p->xa4);
    snd_ContinueAllSoundsInGroup(0xFFFFFFFF);
}
