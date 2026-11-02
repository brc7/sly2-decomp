struct S { int f0; int f4; char pad[0x28]; char x30[0x20]; char x50[0x20]; char x70[0x10]; };
void snd_ResetMovieSound(void);
void Reset__10CByteQueue(void *p);
void Reset__15CQueueOutputIop(void *p);
/** @sly2-annot addr=0x1B3130
 * Purpose: reset byte queues and IOP queue
 * Info: proposed name CMpegAudio_Reset (T1); class CMpegAudio; TU mpeg
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r5_1AE130+verdicts_tu_r3r5+verdicts_A2
 * See: docs/engine/cutscenes.md#5-key-functions, docs/reference/functions/cutscenes.md#mpeg-0x1b2820-0x1b4438
 * @end-sly2-annot */
void Reset__10CMpegAudio(struct S *p)
{
    if (p->f0) {
        snd_ResetMovieSound();
        p->f0 = 1;
        p->f4 = -1;
        Reset__10CByteQueue(p->x30);
        Reset__10CByteQueue(p->x50);
        Reset__15CQueueOutputIop(p->x70);
    }
}
