struct SemaParam { int count; int maxCount; int initCount; int numWaitThreads; unsigned int attr; unsigned int option; };
void *func_002333B4(void *p, int c, int n);
int func_00234A20(struct SemaParam *sp);
/** @sly2-annot addr=0x21EFD0
 * Purpose: not written up yet; only the symbol name (pre-existing / Sly 1-derived) describes it.
 * Confidence: mid (draft) - evidence: pre-existing symbol name only; behaviour not reviewed
 * See: docs/engine/core-library.md#6-source-files, docs/engine/main-loop-startup.md#6-threads,
 *   docs/reference/functions/core-library.md#thread-0x21efd0-0x21f2e0
 * @end-sly2-annot */
int SemaCreate__Fii(int initCount, int maxCount)
{
    struct SemaParam sp;
    func_002333B4(&sp, 0, sizeof(sp));
    sp.initCount = initCount;
    sp.maxCount = maxCount;
    return func_00234A20(&sp);
}
