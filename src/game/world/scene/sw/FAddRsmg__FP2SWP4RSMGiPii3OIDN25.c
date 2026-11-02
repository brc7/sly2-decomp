typedef int OID;
typedef struct RSMG { OID oidRoot; OID oidSM; OID oidTrigger; OID oidNoTrigger; } RSMG;
int FAddRsmg(void *pv, RSMG *arsmg, int crsmgMax, int *pcrsmg, int fTrigger, OID oidRoot, OID oidSM, OID oid)
    __asm__("FAddRsmg__FP2SWP4RSMGiPii3OIDN25");
/** @sly2-annot addr=0x20FEC0
 * Purpose: Add/update a (root,SM)->trigger/untrigger goal record
 * Info: proposed name FAddRsmg__FP2SWP4RSMGiPii3OIDN25 (EXISTING); TU sw
 * Notes: RENAME proposed -> FAddRsmg__FP2SWP4RSMGiPii3OIDN25 (existing_name_changes.csv)
 * Confidence: high [EXISTING 0.80] - evidence: arbitration; pre-existing symbol in symbol_addrs.txt; sources:
 *   r7_1F8DD8+verdicts_tu_r7r8+arbitration
 * See: docs/engine/world-sw.md#10-key-functions, docs/reference/functions/world-sw.md#sw-0x20d9d0-0x210eb0
 * @end-sly2-annot */
int FAddRsmg(void *pv, RSMG *arsmg, int crsmgMax, int *pcrsmg, int fTrigger, OID oidRoot, OID oidSM, OID oid)
{
    int i;
    for (i = 0; i < *pcrsmg; i++) {
        RSMG *prsmg = &arsmg[i];
        if (prsmg->oidRoot == oidRoot && (prsmg->oidSM == -1 || prsmg->oidSM == oidSM)) {
            if (fTrigger)
                prsmg->oidTrigger = oid;
            else
                prsmg->oidNoTrigger = oid;
            return 1;
        }
    }
    if (*pcrsmg + 1 >= crsmgMax)
        return 0;
    i = (*pcrsmg)++;
    arsmg[i].oidRoot = oidRoot;
    arsmg[i].oidSM = oidSM;
    if (fTrigger) {
        arsmg[i].oidTrigger = oid;
        arsmg[i].oidNoTrigger = -1;
    } else {
        arsmg[i].oidNoTrigger = oid;
        arsmg[i].oidTrigger = -1;
    }
    return 1;
}
