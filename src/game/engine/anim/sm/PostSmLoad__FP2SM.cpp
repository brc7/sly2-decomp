struct LO;
struct SMA { int oid; int a; int b; };
struct SM { char pad[0x74]; SMA *psma; };
struct G;
extern "C" {
extern G D_003DD490;
void PostLoLoad(LO *plo); /* PostLoLoad */
LO *PloFindSwObject__FP2SWi3OIDP2LO(G *pg, int kind, int oid, SM *psm);
void func_001F0380(SM *psm, LO *plo, int a, int b);
}
/** @sly2-annot addr=0x1F00C8
 * Purpose: PostLoLoad; if a pending apply record exists, find its root ALO by OID and PsmaApplySm
 * Info: proposed name PostSmLoad__FP2SM (EXISTING); class SM slot15; TU sm
 * Confidence: high [EXISTING 0.85] - evidence: semantic review; pre-existing symbol in symbol_addrs.txt;
 *   sources: semantic_tu_docs/sm
 * See: docs/engine/animation.md#5-state-machines-sm-and-sma-smcpp-0x1efa90-0x1f1768,
 *   docs/engine/animation.md#7-key-functions, docs/reference/functions/animation.md#sm-0x1efa90-0x1f1768
 * @end-sly2-annot */
void PostSmLoad(SM *psm)
{
    PostLoLoad((LO *)psm);
    if (psm->psma) {
        LO *plo = PloFindSwObject__FP2SWi3OIDP2LO(&D_003DD490, 4, psm->psma->oid, psm);
        func_001F0380(psm, plo, psm->psma->a, psm->psma->b);
    }
}
