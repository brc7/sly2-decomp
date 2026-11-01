typedef struct MSG { int cb; int nKind; } MSG;
typedef struct POOL { int cb; int c; int nKind; void *pvFree; int cAlloc; void *pvBlock; } POOL;

void *func_001AE1D0(MSG *pmsg);
void *func_002333B4(void *pv, int b, unsigned int cb);

/** @sly2-annot addr=0x11CCC0
 * Purpose: Pops a slot (allocating c*cb from the level heap 0x1AE1D0 and threading the free list on first
 *   use), increments count, memsets to 0
 * Info: proposed name PvAllocSlotheapClear (T1); TU slotheap
 * Notes: names_v2 C proposed StartAloSound__FP3ALO5SFXIDfffP2LM (0.167)
 * Confidence: mid (draft) [T1 0.60] - evidence: static naming pass; sources: r1_116918
 * See: docs/reference/functions/core-library.md#slotheap-0x11cca8-0x11cf60,
 *   docs/reference/functions/splice-scripting.md#ref-0x1291a8-0x12a3d8
 * @end-sly2-annot */
void *func_0011CCC0(POOL *ppool)
{
    void *pv;

    if (ppool->pvFree == 0) {
        MSG msg;
        char *ab;
        int i;

        msg.cb = ppool->c * ppool->cb;
        msg.nKind = ppool->nKind;
        ab = (char *)func_001AE1D0(&msg);
        for (i = 1; i < ppool->c; i++) {
            *(void **)(ab + (i - 1) * ppool->cb) = ab + i * ppool->cb;
        }
        *(void **)(ab + (i - 1) * ppool->cb) = 0;
        ppool->pvFree = ab;
        ppool->pvBlock = ab;
    }

    pv = ppool->pvFree;
    ppool->pvFree = *(void **)pv;
    func_002333B4(pv, 0, ppool->cb);
    ppool->cAlloc++;
    return pv;
}
