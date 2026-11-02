struct S { int state; int x4; char x8[0x28]; char x30[0x10]; int x40; char pad[0xC]; char x50[0x10]; int x60; char pad2[0xC]; char x70[0x10]; };
int func_0013D7D0(void *p, int cb, unsigned char *pb);
int CQueueOutputIop_Init(void *a, void *b);
/** @sly2-annot addr=0x1B31E0
 * Purpose: accept demuxed audio packet: pick stream id, init queue from header, CbFill
 * Info: proposed name CMpegAudio_FAccept (T1); class CMpegAudio; TU mpeg
 * Confidence: high [T1 0.80] - evidence: static analysis; sources: r5_1AE130
 * See: docs/engine/cutscenes.md#5-key-functions, docs/reference/functions/cutscenes.md#mpeg-0x1b2820-0x1b4438
 * @end-sly2-annot */
int FAccept__10CMpegAudioiPUc(struct S *p, int cb, unsigned char *pb)
{
    int n;
    int id;
    if (p->state == 0) return 0;
    id = pb[3];
    if (p->x4 == -1) p->x4 = id;
    if (p->x4 != id) return 1;
    cb -= 4;
    pb += 4;
    if (p->state == 1) {
        n = func_0013D7D0(p->x30, cb, pb);
        cb -= n;
        pb += n;
        if (p->x40 > 0) return 1;
        if (CQueueOutputIop_Init(p->x70, p->x8)) p->state = 2;
        else p->state = 0;
    }
    if (p->x60 >= cb) {
        func_0013D7D0(p->x50, cb, pb);
        return 1;
    }
    return 0;
}
