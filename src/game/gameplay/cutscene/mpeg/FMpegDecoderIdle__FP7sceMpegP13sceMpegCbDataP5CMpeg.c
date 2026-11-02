void CbDemuxed__5CMpegi(void *p, int n);
void Update__10CMpegAudio(void *p);
/** @sly2-annot addr=0x1B34D0
 * Purpose: idle callback: spool stream + audio update
 * Info: proposed name FMpegDecoderIdle (T1); TU mpeg
 * Confidence: mid (draft) [T1 0.75] - evidence: static analysis; sources: r5_1AE130
 * See: docs/engine/cutscenes.md#5-key-functions, docs/reference/functions/cutscenes.md#mpeg-0x1b2820-0x1b4438
 * @end-sly2-annot */
int FMpegDecoderIdle__FP7sceMpegP13sceMpegCbDataP5CMpeg(void *a, void *b, char *p)
{
    CbDemuxed__5CMpegi(p, 0);
    Update__10CMpegAudio(p + 0x180);
    return 1;
}
