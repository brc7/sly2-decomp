typedef union QW { unsigned int aun[4]; unsigned short aus[8]; } QW;
typedef struct ALLOCREQ { int cb; int nKind; } ALLOCREQ;
struct X { int pad[4]; int cqw; };
struct Y { int pad[4]; QW *aqw; char *pb; };
QW *func_001AE238(ALLOCREQ *preq, QW *aqw);
/** @sly2-annot addr=0x169EB8
 * Purpose: Copies a VIF/DMA packet into the SW heap and optionally relocates DMA REF addresses (tag id 4) by
 *   a delta
 * Info: proposed name CopyRelocateDmaChain (T1); TU glob
 * Notes: verifier PLAUSIBLE: -> CopyRelocateDmaChain
 * Confidence: mid (draft) [T1 0.60] - evidence: static analysis; sources: r3_1539A8+verdicts_tu_r3r5
 * See: docs/engine/rendering.md#13-key-functions,
 *   docs/guides/port-plan.md#11-known-facts-that-drive-the-design,
 *   docs/reference/functions/rendering.md#glob-0x167310-0x16f148
 * @end-sly2-annot */
void CopyRelocateDmaChain(struct X *px, struct Y *py, int cqw, QW *aqwSrc, int fReloc, int nNew, int nOld)
{
    ALLOCREQ req;
    QW *aqw;
    req.cb = cqw << 4;
    req.nKind = 0x10;
    px->cqw = cqw;
    aqw = func_001AE238(&req, aqwSrc);
    py->aqw = aqw;
    py->pb += (char *)aqw - (char *)aqwSrc;
    if (fReloc) {
        int dn = nNew - nOld;
        QW *pqw = aqw;
        while (pqw < py->aqw + px->cqw) {
            switch ((int)(pqw->aun[0] >> 28)) {
            case 4:
                pqw->aun[1] += dn;
                pqw++;
                break;
            case 1:
                pqw += pqw->aus[0] + 1;
                break;
            case 6:
                pqw = py->aqw + px->cqw;
                break;
            }
        }
    }
}
