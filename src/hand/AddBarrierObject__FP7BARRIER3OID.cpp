enum OID { OID_0 };
struct BARRIER { char pad[0x150]; unsigned int coid; OID aoid[4]; unsigned int ccid; int acid[4]; };
/** @sly2-annot addr=0x1323C8
 * Purpose: not written up yet; only the symbol name (pre-existing / Sly 1-derived) describes it.
 * Confidence: mid (draft) - evidence: pre-existing symbol name only; behaviour not reviewed
 * See: docs/reference/functions/gameplay-objects.md#barrier-0x1320c8-0x132570
 * @end-sly2-annot */
void AddBarrierObject(BARRIER *pbarrier, OID oid)
{
    if (pbarrier->coid < 4) pbarrier->aoid[pbarrier->coid++] = oid;
}
