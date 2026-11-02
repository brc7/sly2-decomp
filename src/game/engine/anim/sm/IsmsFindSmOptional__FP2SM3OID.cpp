enum OID { OID_Nil = -1 };
struct SMS { OID oid; int n; };
struct SM { char pad[0x50]; int csms; SMS *asms; };
/** @sly2-annot addr=0x1F0400
 * Purpose: Index of the state with that OID, or -1
 * Info: proposed name IsmsFindSmOptional__FP2SM3OID (EXISTING); TU sm
 * Confidence: high [EXISTING 0.85] - evidence: semantic review; pre-existing symbol in symbol_addrs.txt;
 *   sources: semantic_tu_docs/sm
 * See: docs/engine/animation.md#5-state-machines-sm-and-sma-smcpp-0x1efa90-0x1f1768,
 *   docs/reference/functions/animation.md#sm-0x1efa90-0x1f1768
 * @end-sly2-annot */
int IsmsFindSmOptional(SM *psm, OID oid)
{
    int isms;
    for (isms = 0; isms < psm->csms; isms++)
        if (psm->asms[isms].oid == oid) return isms;
    return -1;
}
