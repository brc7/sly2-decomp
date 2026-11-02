struct S { char pad[0x160]; char x160[0x14]; int x174; char pad2[8]; char x180[0x10]; };
extern char D_002D4220[];
int CbDemuxed__5CMpegi(struct S *p, int n);
void Update__10CMpegAudio(void *p);
void func_002333B4(void *p, int c, int n);
void CbFill__10CByteQueueiPUc(void *p, int n, void *b);
void FreeDrain__10CByteQueuei(void *p, int n);
void CbDrain__10CByteQueueiP12CQueueOutput(void *p, int n, void *b);
/** @sly2-annot addr=0x1B33F0
 * Purpose: decode callback: feed IPU from video queue
 * Info: proposed name FMpegDecodeVideo (T1); TU mpeg
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r5_1AE130+verdicts_tu_r3r5+verdicts_A2
 * See: docs/engine/cutscenes.md#5-key-functions, docs/reference/functions/cutscenes.md#mpeg-0x1b2820-0x1b4438
 * @end-sly2-annot */
int FMpegDecodeVideo__FP7sceMpegP13sceMpegCbDataP5CMpeg(int a, int b, struct S *p)
{
    void *buf[16];
    while (p->x174 < 0x1000) {
        int v = CbDemuxed__5CMpegi(p, 1);
        if (v == 2) {
            func_002333B4(buf, 0, 0x40);
            CbFill__10CByteQueueiPUc(p->x160, 0x40 - p->x174 % 32, buf);
            break;
        }
        if (v == 1) Update__10CMpegAudio(p->x180);
    }
    buf[0] = D_002D4220;
    FreeDrain__10CByteQueuei(p->x160, 0);
    CbDrain__10CByteQueueiP12CQueueOutput(p->x160, 0, buf);
    return 1;
}
