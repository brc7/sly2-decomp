typedef struct X { char pad[0xC]; int nC; int n10; int n14; } X;

/** @sly2-annot addr=0x11CDB8
 * Purpose: Clear a slot heap (+0xC count, +0x10 free list, +0x14 block) so it re-allocates
 * Info: proposed name ResetSlotheap (T1); TU slotheap
 * Notes: names_v2 C proposed ResetGsb__FP3GSB (0.200) | top50_round (0.60): Body; callers ShutdownSplice
 *   0x12AE28, ResetSndmgr 0x1FD700 (per-level teardown). Sly 1 slotheap API (InitSlotheap/PvAllocSlotheap*
 *   /FreeSlotheapPv) has no reset; coined in that style
 * Confidence: mid (draft) [T1 0.60] - evidence: Sly 1 / Lilly reference; semantic review; static naming pass;
 *   sources: r1_116918+top50_round
 * See: docs/reference/functions/core-library.md#slotheap-0x11cca8-0x11cf60,
 *   docs/reference/functions/splice-scripting.md#spliceutils-0x12ae18-0x12b0e8
 * @end-sly2-annot */
void func_0011CDB8(X *px)
{
    px->n14 = 0;
    px->n10 = 0;
    px->nC = 0;
}
