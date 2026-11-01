// slotheap.cpp -- fixed-size slot allocator. TU 0x11CCA8..0x11CF60 (7 functions, all matching).
// SLY2_ALIAS: __static_initialization_and_destruction_0=StaticInit_slotheap
// SLY2_ALIAS: _GLOBAL_$I$*=_GLOBAL_$I$slotheap
#include "sly2/slotheap.h"

void *PvAllocSwImpl(ALLOCREQ *preq);                 /* world heap bump allocator (0x1AE1D0) */
extern "C" void *func_002333B4(void *pv, int b, unsigned int cb);   /* memset; a prototype named memset becomes a builtin */

SLOTHEAP *InitSlotheap(SLOTHEAP *pslotheap, int cb, int c, int cbAlign)
{
    pslotheap->cb = cb;
    pslotheap->c = c;
    pslotheap->cbAlign = cbAlign;
    pslotheap->pslotFree = 0;
    return pslotheap;
}

void *PvAllocSlotheapClear(SLOTHEAP *pslotheap)
{
    SLOT *pslot;
    if (pslotheap->pslotFree == 0) {
        // First allocation: carve one block into c slots linked through their first word.
        ALLOCREQ req;
        char *ab;
        int i;
        req.cb = pslotheap->c * pslotheap->cb;
        req.cbAlign = pslotheap->cbAlign;
        ab = (char *)PvAllocSwImpl(&req);
        for (i = 1; i < pslotheap->c; i++) {
                ((SLOT *)(ab + (i - 1) * pslotheap->cb))->pslotNext = (SLOT *)(ab + i * pslotheap->cb);
        }
        ((SLOT *)(ab + (i - 1) * pslotheap->cb))->pslotNext = 0;
        pslotheap->pslotFree = (SLOT *)ab;
        pslotheap->ab = (byte *)ab;
    }
    pslot = pslotheap->pslotFree;
    pslotheap->pslotFree = pslot->pslotNext;
    func_002333B4(pslot, 0, pslotheap->cb);
    pslotheap->cAlloc++;
    return pslot;
}

void FreeSlotheapPv(SLOTHEAP *pslotheap, void *pv)
{
    pslotheap->cAlloc--;
    SLOT *pslot = (SLOT *)pv;
    pslot->pslotNext = pslotheap->pslotFree;
    pslotheap->pslotFree = pslot;
}

void ResetSlotheap(SLOTHEAP *pslotheap)
{
    pslotheap->ab = 0;
    pslotheap->cAlloc = 0;
    pslotheap->pslotFree = 0;
}

// Allocate and immediately free one slot so the block exists before gameplay.
void PrimeSlotheap(SLOTHEAP *pslotheap)
{
    FreeSlotheapPv(pslotheap, PvAllocSlotheapClear(pslotheap));
}

SLY2_INSTANTIATE_SLOT_POOLS()
