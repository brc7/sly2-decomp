typedef struct MSG { int n; int nKind; } MSG;
void func_001AE1D0(MSG *pmsg);

/** @sly2-annot addr=0x116A30
 * Purpose: Heap vtable slot 1 for the global level heap object D_002DD590: allocates cb bytes 16-aligned from
 *   the SW bump allocator 0x1AE1D0 (alignment arg ignored)
 * Info: proposed name CSwHeap_PvAlloc (T1); class CSwHeap; TU heap
 * Confidence: mid (draft) [T1 0.55] - evidence: static analysis; sources: r1_116918
 * See: docs/reference/functions/core-library.md#heap-0x116918-0x116ca0
 * @end-sly2-annot */
void CSwHeap_PvAlloc(void *pv, int n)
{
    MSG msg;
    msg.n = n;
    msg.nKind = 0x10;
    func_001AE1D0(&msg);
}
