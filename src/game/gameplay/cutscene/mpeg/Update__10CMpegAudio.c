struct S { int state; char pad[0x4C]; char x50[0x20]; int x70; int x74; int x78; int x7C; int x80; int x84; int x88; int x8C; int x90; int x94; };
int CbDrain__10CByteQueueiP12CQueueOutput(void *a, int b, void *c);
void snd_StartMovieSound(int a, int b, int c, int d, int e);
void CQueueOutputIop_Update(void *p);
/** @sly2-annot addr=0x1B32B8
 * Purpose: drain byte queue to IOP per state
 * Info: proposed name CMpegAudio_Update (T1); class CMpegAudio; TU mpeg
 * Confidence: high [T1 0.80] - evidence: static analysis; sources: r5_1AE130+verdicts_A2
 * See: docs/engine/cutscenes.md#5-key-functions, docs/reference/functions/cutscenes.md#mpeg-0x1b2820-0x1b4438
 * @end-sly2-annot */
void Update__10CMpegAudio(struct S *p)
{
    switch (p->state) {
    case 2:
        CbDrain__10CByteQueueiP12CQueueOutput(p->x50, p->x94, &p->x70);
        if (p->x94 == 0) {
            if (p->x74 == 0x10) snd_StartMovieSound(p->x84, 0x1000, 0, p->x78, p->x7C);
            else snd_StartMovieSound(p->x84, p->x88, p->x84, 0, 0);
            p->state = 3;
        }
        break;
    case 3:
        CQueueOutputIop_Update(&p->x70);
        CbDrain__10CByteQueueiP12CQueueOutput(p->x50, p->x94, &p->x70);
        break;
    }
}
