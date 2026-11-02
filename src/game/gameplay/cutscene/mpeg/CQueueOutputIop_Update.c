struct S { int x0; int x4; int x8; int xC; int x10; int x14; int x18; int x1C; int x20; int x24; int x28; };
int func_00233018(void);
unsigned int snd_GetTransStatus(void);
/** @sly2-annot addr=0x1B2EB8
 * Purpose: Read IOP playback position and update read/write cursors
 * Info: proposed name CQueueOutputIop_Update (T1); class CQueueOutputIop; TU mpeg
 * Confidence: mid (draft) [T1 0.75] - evidence: static analysis; sources: r5_1AE130
 * See: docs/engine/cutscenes.md#5-key-functions, docs/reference/functions/cutscenes.md#mpeg-0x1b2820-0x1b4438
 * @end-sly2-annot */
void CQueueOutputIop_Update(struct S *p)
{
    if (p->x4 == 0x10) {
        int pos = func_00233018() * p->xC;
        if (pos >= 0 && pos <= p->x18) {
            p->x20 = pos % p->x18;
            if (p->x20 == p->x1C) p->x24 = p->x18;
            else {
                p->x24 = p->x20 - p->x1C + p->x18;
                p->x24 %= p->x18;
            }
        }
        p->x28 = p->x18 - p->x24;
    } else {
        p->x24 = (snd_GetTransStatus() & 0xFFFFFF) - p->x14 - p->x1C + (p->x18 - 0x400);
        p->x24 %= p->x18;
        p->x28 = p->x18 - p->x24;
        p->x20 = p->x1C - p->x28 + p->x18;
        p->x20 %= p->x18;
    }
}
