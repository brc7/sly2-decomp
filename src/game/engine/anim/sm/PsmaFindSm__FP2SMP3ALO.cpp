struct ALO;
struct SMA { char pad[0x14]; SMA *psmaNext; char pad2[0x10]; ALO *palo; };
struct SM { char pad[0x68]; SMA *psmaFirst; };
/** @sly2-annot addr=0x1F03C8
 * Purpose: Linear search of the SMA list by root ALO
 * Info: proposed name PsmaFindSm__FP2SMP3ALO (EXISTING); TU sm
 * Confidence: high [EXISTING 0.85] - evidence: semantic review; pre-existing symbol in symbol_addrs.txt;
 *   sources: semantic_tu_docs/sm
 * See: docs/engine/animation.md#5-state-machines-sm-and-sma-smcpp-0x1efa90-0x1f1768,
 *   docs/reference/functions/animation.md#sm-0x1efa90-0x1f1768
 * @end-sly2-annot */
SMA *PsmaFindSm(SM *psm, ALO *palo)
{
    int unused[2];
    SMA *psma;
    for (psma = psm->psmaFirst; psma; psma = psma->psmaNext)
        if (psma->palo == palo) return psma;
    return 0;
}
