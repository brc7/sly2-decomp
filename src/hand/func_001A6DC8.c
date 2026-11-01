typedef unsigned long ulong;
struct O { char pad[0x24]; struct O *pNext; ulong grf; char pad2[0x34]; struct O *pFirst; };
void func_001A6D28(struct O *, int);
/** @sly2-annot addr=0x1A6DC8
 * Purpose: SO vt46: grfvis=b and propagate to children without bit58
 * Info: proposed name SetSoVisMask (T1); class SO slot46; TU lo
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: r4_17F090
 * See: docs/reference/functions/object-model.md#lo-0x1a5270-0x1a7518
 * @end-sly2-annot */
void func_001A6DC8(struct O *p, int b)
{
    struct O *q;
    *((unsigned char *)p + 0x2D) = b;
    for (q = p->pFirst; q; q = q->pNext) {
        if (!(q->grf & (1UL << 58)))
            func_001A6D28(q, b);
    }
}
