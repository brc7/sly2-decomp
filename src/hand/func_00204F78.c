struct G;
extern struct G D_003DD490;
struct O { char pad[0x10]; struct O *pParent; char p2[0xEC - 0x14]; void *pvEC; };
void *func_001A5BF0(void *pv);
void func_00133A38(struct G *pg, void *a, void *b, int n);
void func_001F4BF8(struct O *p);
/** @sly2-annot addr=0x204F78
 * Purpose: Find the SO with an attachment record up plo's parent chain and link/unlink (0x133A38 g_sw, f?1:2)
 *   this step's attachment with it, then 0x1F4BF8
 * Info: proposed name LinkStepAttachment (T2); TU step
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r7b_guard
 * See: docs/reference/functions/guards-ai.md#step-0x203948-0x205e70
 * @end-sly2-annot */
void func_00204F78(struct O *p, void *pv, int f)
{
    struct O *o = func_001A5BF0(pv);
    while (o && !o->pvEC) o = o->pParent;
    if (p->pvEC && o && o->pvEC) func_00133A38(&D_003DD490, p->pvEC, o->pvEC, f ? 1 : 2);
    func_001F4BF8(p);
}
