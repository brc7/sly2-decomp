#include "common.h"
struct Q { char pad[0x10]; ulong fl; };
struct E { int k; int pad[2]; int fl; int pad2[5]; struct Q *pq; };
struct P { char pad[0xB8]; int c; int pad2; struct E *ae; };
/** @sly2-annot addr=0x1F6978
 * Purpose: For every glob with id k: clear hidden flag 0x800 and clear the linked SOPART hide bit 50
 * Info: proposed name ShowSoGlobsById (T1); TU so
 * Notes: top50_round (0.60): Body; exact inverse of HideSoGlobsById 0x1F69F8. Splice option HideSoPartsById*
 *   family and CID98 stage actions AddCid98ShowGlobs use it
 * Confidence: mid (draft) [T1 0.60] - evidence: semantic review; sources: r6_1D5150+top50_round
 * See: docs/engine/animation.md#open-questions,
 *   docs/reference/functions/game-progression.md#prize-0x1c4e50-0x1c8cc0,
 *   docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8
 * @end-sly2-annot */
void ShowSoGlobsById(struct P *p, int k)
{
    int i;
    for (i = 0; i < p->c; i++) {
        struct E *pe = &p->ae[i];
        if (pe->k == k) {
            pe->fl &= ~0x800;
            if (pe->pq) pe->pq->fl &= ~(1L << 50);
        }
    }
}
