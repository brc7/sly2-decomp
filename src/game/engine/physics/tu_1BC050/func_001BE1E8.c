struct DL { void *pvFirst; void *pvLast; int ibDle; };
extern struct DL D_003E1A4C;
void RemoveDlEntry__FP2DLPv(struct DL *pdl, void *pv);
void func_001BD040(void *pv);
int FIsDlEmpty__FP2DL(struct DL *pdl);
/** @sly2-annot addr=0x1BE1E8
 * Purpose: Drain pending list D_003E1A4C registering each obstacle
 * Info: proposed name UpdatePendingCid14Obstacles (T2); class CID14; TU tu_1BC050
 * Notes: names_v2 B proposed FIsJtSoundBase__FP2JT (0.856)
 * Confidence: low (draft) [T2 0.50] - evidence: static naming pass; sources: r5b_rest
 * See: docs/reference/functions/physics-collision.md#tu_1bc050-0x1bc050-0x1be398
 * @end-sly2-annot */
void func_001BE1E8(void)
{
    while (!FIsDlEmpty__FP2DL(&D_003E1A4C)) {
        void *pv = D_003E1A4C.pvFirst;
        RemoveDlEntry__FP2DLPv(&D_003E1A4C, pv);
        func_001BD040(pv);
    }
}
