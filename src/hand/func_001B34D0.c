void func_001B41D0(void *p, int n);
void func_001B32B8(void *p);
/** @sly2-annot addr=0x1B34D0
 * Purpose: idle callback: spool stream + audio update
 * Info: proposed name FMpegDecoderIdle (T1); TU mpeg
 * Confidence: mid (draft) [T1 0.75] - evidence: static analysis; sources: r5_1AE130
 * See: docs/engine/cutscenes.md#5-key-functions, docs/reference/functions/cutscenes.md#mpeg-0x1b2820-0x1b4438
 * @end-sly2-annot */
int func_001B34D0(void *a, void *b, char *p)
{
    func_001B41D0(p, 0);
    func_001B32B8(p + 0x180);
    return 1;
}
