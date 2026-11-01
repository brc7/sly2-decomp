struct DL { void *pvFirst; void *pvLast; int ibDle; };
struct O { char pad[0x1C4]; struct DL dl; char pad2[0x1D8 - 0x1D0]; void *powner; };
extern struct DL D_003E1A4C;
void func_0011A220(struct DL *pdl, void *pv);
void func_0011A2E8(struct DL *pdl, void *pv);
int func_0011A380(struct DL *pdl, void *pv);
void func_001BD1E0(void *p);
/** @sly2-annot addr=0x1BDE18
 * Purpose: Attach polygon to merged owner and unregister it individually
 * Info: proposed name AddCid14ToMerged (T2); class CID14; TU tu_1BC050
 * Confidence: low (draft) [T2 0.35] - evidence: static analysis; sources: r5b_rest
 * See: docs/reference/functions/physics-collision.md#tu_1bc050-0x1bc050-0x1be398
 * @end-sly2-annot */
void func_001BDE18(void *p, struct O *po)
{
    struct DL *pdl;
    *(struct O **)((char *)p + 0x1D8) = po;
    func_0011A220(&po->dl, p);
    func_001BD1E0(p);
    pdl = &D_003E1A4C;
    if (func_0011A380(pdl, p)) func_0011A2E8(pdl, p);
}
