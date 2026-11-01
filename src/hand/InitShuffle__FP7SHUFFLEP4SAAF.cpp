struct SAAF { char pad[0xC]; float dtLoopMin; float dtLoopMax; };
struct SHUFFLE { char pad[0x50]; float dtPauseMin; float dtPauseMax; };
void InitSaa(SHUFFLE *p, SAAF *psaaf);
/** @sly2-annot addr=0x1EDFA0
 * Purpose: not written up yet; only the symbol name (pre-existing / Sly 1-derived) describes it.
 * Confidence: mid (draft) - evidence: pre-existing symbol name only; behaviour not reviewed
 * See: docs/reference/functions/rendering.md#shdanim-0x1ed7a8-0x1ee988
 * @end-sly2-annot */
void InitShuffle(SHUFFLE *pshuffle, SAAF *psaaf)
{
    float dtMax, dtMin;
    InitSaa(pshuffle, psaaf);
    dtMin = psaaf->dtLoopMin;
    dtMax = psaaf->dtLoopMax;
    pshuffle->dtPauseMax = dtMax;
    pshuffle->dtPauseMin = dtMin;
}
