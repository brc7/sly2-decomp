struct CRITSECT { int cEnter; int thread; int sema; };
extern "C" int func_00234910(void);
extern "C" int func_00234A60(int sema);

/** @sly2-annot addr=0x21F060
 * Purpose: not written up yet; only the symbol name (pre-existing / Sly 1-derived) describes it.
 * Confidence: mid (draft) - evidence: pre-existing symbol name only; behaviour not reviewed
 * See: docs/reference/functions/core-library.md#thread-0x21efd0-0x21f2e0
 * @end-sly2-annot */
void EnterCritSect(CRITSECT *pcritsect)
{
    int threadId = func_00234910();

    if (threadId != pcritsect->thread)
    {
        func_00234A60(pcritsect->sema);
        pcritsect->thread = threadId;
        pcritsect->cEnter = 1;
    }
    else
    {
        pcritsect->cEnter++;
    }
}
