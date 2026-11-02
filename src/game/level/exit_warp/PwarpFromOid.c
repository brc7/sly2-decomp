struct O { char pad[0x18]; int id; char pad1[0x16C - 0x1C]; int c; int aid[1]; };
struct O *PloFindSwObject__FP2SWi3OIDP2LO(void *pg, int k, int id, struct O *p);
int CploFindSwObjectsByClass(void *pg, int a, int b, struct O *po, int n, struct O **apo);
/** @sly2-annot addr=0x227FE8
 * Purpose: finds a WARP by oid (0x15E938) else searches class 0x2A (WARP) by id or alias list +0x16C
 *   (0x15E9C8(sw, a, 0x2A, ...)); Sly 1 WARP oidAlias lookup
 * Info: proposed name PwarpFromOid (T1); TU exit_warp
 * Notes: v3 purpose fix (class review): falls back over cid 0x2A WARP, not 0x1A
 * Confidence: mid (draft) [T1 0.50] - evidence: static naming pass; sources:
 *   r8_21BB88+class_review(master_names_v2_1)
 * See: docs/engine/physics-collision.md#6-source-files,
 *   docs/engine/world-sw.md#4-transitions-warps-exits-wipes-boot-queue,
 *   docs/engine/world-sw.md#9-source-files
 * @end-sly2-annot */
struct O *PwarpFromOid(void *pg, int id, int k)
{
    struct O *apo[32];
    struct O *poRet;
    struct O *po = 0;
    int n, i;
    int a = 5;
    if (k != -1) {
        po = PloFindSwObject__FP2SWi3OIDP2LO(pg, 5, k, 0);
        if (po) a = 1;
    }
    poRet = 0;
    n = CploFindSwObjectsByClass(pg, a, 0x2A, po, 0x20, apo);
    for (i = 0; i < n; i++) {
        struct O *pe = apo[i];
        int j;
        if (pe->id == id) goto found;
        for (j = 0; j < pe->c; j++) {
            if (pe->aid[j] == id) goto found;
        }
        continue;
found:
        poRet = pe;
    }
    return poRet;
}
