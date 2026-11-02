struct S { char pad[0x10C]; int x10C; int x110; int x114; char x118[0x48]; char x160[0x20]; char x180[0x10]; };
extern int D_002E1E54, D_002DEDBC;
void Init__10CByteQueueiPv(void *p, int cb, unsigned int a);
void Init__10CMpegAudio(void *p);
int sceMpegInit(void);
void sceMpegCreate(void *p, int a, int b);
void sceMpegAddStrCallback(void *p, int a, int b, void *fn, struct S *s);
void sceMpegAddCallback(void *p, int a, void *fn, struct S *s);
int CbDemuxed__5CMpegi(struct S *p, int n);
void FMpegAcceptVideo__FP7sceMpegP16sceMpegCbDataStrP5CMpeg(void);
void FMpegAcceptAudio__FP7sceMpegP16sceMpegCbDataStrP5CMpeg(void);
void FMpegDecodeVideo__FP7sceMpegP13sceMpegCbDataP5CMpeg(void);
void FMpegDecoderIdle__FP7sceMpegP13sceMpegCbDataP5CMpeg(void);
void FMpegDecoderError__FP7sceMpegP13sceMpegCbDataP5CMpeg(void);
/** @sly2-annot addr=0x1B3EC0
 * Purpose: init byte queue, CMpegAudio::Init, sceMpegInit/Create, add callbacks
 * Info: proposed name CMpeg_Start (T1); class CMpeg; TU mpeg
 * Confidence: high [T1 0.80] - evidence: static analysis; sources: r5_1AE130
 * See: docs/engine/cutscenes.md#5-key-functions, docs/reference/functions/cutscenes.md#mpeg-0x1b2820-0x1b4438
 * @end-sly2-annot */
void CMpeg_Start(struct S *p, int a)
{
    p->x114 = a;
    p->x10C = 0;
    Init__10CByteQueueiPv(p->x160, 0x40000, (p->x110 + 0x245A80) | 0x20000000);
    Init__10CMpegAudio(p->x180);
    sceMpegInit();
    sceMpegCreate(p->x118, p->x110 + 0x98000, 0x1ADA60);
    sceMpegAddStrCallback(p->x118, 0, 0, FMpegAcceptVideo__FP7sceMpegP16sceMpegCbDataStrP5CMpeg, p);
    sceMpegAddStrCallback(p->x118, 2, 0, FMpegAcceptAudio__FP7sceMpegP16sceMpegCbDataStrP5CMpeg, p);
    sceMpegAddStrCallback(p->x118, 3, 0, FMpegAcceptAudio__FP7sceMpegP16sceMpegCbDataStrP5CMpeg, p);
    sceMpegAddStrCallback(p->x118, 2, D_002E1E54 + 1, FMpegAcceptAudio__FP7sceMpegP16sceMpegCbDataStrP5CMpeg, p);
    sceMpegAddStrCallback(p->x118, 3, D_002E1E54 + 1, FMpegAcceptAudio__FP7sceMpegP16sceMpegCbDataStrP5CMpeg, p);
    sceMpegAddCallback(p->x118, 1, FMpegDecodeVideo__FP7sceMpegP13sceMpegCbDataP5CMpeg, p);
    sceMpegAddCallback(p->x118, 4, FMpegDecoderIdle__FP7sceMpegP13sceMpegCbDataP5CMpeg, p);
    sceMpegAddCallback(p->x118, 0, FMpegDecoderError__FP7sceMpegP13sceMpegCbDataP5CMpeg, p);
    while (!CbDemuxed__5CMpegi(p, 1)) ;
    D_002DEDBC = 0;
}
