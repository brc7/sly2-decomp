struct O { char pad[0x130]; int f; };
extern char D_003DD490[];
int CploFindSwObjectsByClass(void *g, int a, int b, int c, int n, struct O **ap);
/** @sly2-annot addr=0x2280D0
 * Purpose: Return the last EXIT (cid 0x1A) whose +0x130 (Sly 1 EXIT.fDefault) is set
 * Info: proposed name PexitDefault (T1); TU exit_warp
 * Notes: names_v2 C proposed BindStepguard__FP9STEPGUARD (0.638) | v3: superseded PwarpDefault | CONFIRMED
 *   (fields): Sly 1 PexitDefault; v2 PwarpDefault
 * Confidence: mid (draft) [T1 0.75] - evidence: Sly 1 / Lilly reference; static naming pass; sources:
 *   r8_21BB88+class_review(master_names_v2_1)
 * See: docs/engine/world-sw.md#4-transitions-warps-exits-wipes-boot-queue,
 *   docs/reference/functions/world-sw.md#exit_warp-0x227fe8-0x228b48
 * @end-sly2-annot */
struct O *func_002280D0(void)
{
    struct O *ap[32];
    struct O *r = 0;
    int n = CploFindSwObjectsByClass(D_003DD490, 5, 0x1A, 0, 0x20, ap);
    int i;
    for (i = 0; i < n; i++) if (ap[i]->f) r = ap[i];
    return r;
}
