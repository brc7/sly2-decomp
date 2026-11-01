enum CID { CID_0 };
struct BARRIER { char pad[0x150]; unsigned int coid; int aoid[4]; unsigned int ccid; CID acid[4]; };
/** @sly2-annot addr=0x1323F0
 * Purpose: not written up yet; only the symbol name (pre-existing / Sly 1-derived) describes it.
 * Confidence: mid (draft) - evidence: pre-existing symbol name only; behaviour not reviewed
 * See: docs/reference/functions/gameplay-objects.md#barrier-0x1320c8-0x132570
 * @end-sly2-annot */
void AddBarrierClass(BARRIER *pbarrier, CID cid)
{
    if (pbarrier->ccid < 4) pbarrier->acid[pbarrier->ccid++] = cid;
}
