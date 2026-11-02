/** @sly2-annot addr=0x1B2FD8
 * Purpose: Kick IPU_TO DMA (0x1000B400/10/20) with up to 0x1000 bytes
 * Info: proposed name CQueueOutputIpu_CbWrite (T1); class CQueueOutputIpu; TU mpeg
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r5_1AE130
 * See: docs/engine/cutscenes.md#5-key-functions, docs/reference/functions/cutscenes.md#mpeg-0x1b2820-0x1b4438
 * @end-sly2-annot */
int func_001B2FD8(void *p, unsigned int cb, unsigned int pv)
{
    int n = cb & 0xFFFFFFF0;
    if (n > 0x1000) n = 0x1000;
    *(volatile unsigned int *)0x1000B420 = (unsigned int)n >> 4;
    *(volatile unsigned int *)0x1000B410 = pv & 0x0FFFFFFF;
    *(volatile unsigned int *)0x1000B400 = 0x101;
    __asm__ __volatile__("sync");
    return n;
}
