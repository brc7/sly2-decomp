struct G;
extern struct G D_003DD490;
struct N { int k; int b; char pad[0x10]; struct N *next; };
struct S { char pad[0x50]; struct N *pn; };
struct S *PloFindSwObject__FP2SWi3OIDP2LO(struct G *pg, int k, int b, struct S *p);
int func_001709A8(struct S *p);
void func_001E9C30(struct S *p);
/** @sly2-annot addr=0x170A58
 * Purpose: Walks the command list: context commands (kind 0) change the lookup context; for move-along-shape
 *   commands (kind 6) whose target is a plain SHAPE it calls 0x1E9C30 on it
 * Info: proposed name PrepareCid113Targets (T2); class CID113; TU tu_1709A8
 * Notes: Cid71->Cid113 (CID 0x71=113, agent-default(hex))
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r3b_rest
 * See: docs/reference/functions/guards-ai.md#tu_1709a8-0x1709a8-0x171f58
 * @end-sly2-annot */
void func_00170A58(struct S *p) {
    struct N *pn;
    for (pn = p->pn; pn; pn = pn->next) {
        switch (pn->k) {
        case 6: {
            struct S *o = PloFindSwObject__FP2SWi3OIDP2LO(&D_003DD490, 4, pn->b, p);
            if (o && func_001709A8(o) == -1) func_001E9C30(o);
            break;
        }
        case 0:
            p = PloFindSwObject__FP2SWi3OIDP2LO(&D_003DD490, p ? 4 : 5, pn->b, p);
            break;
        }
    }
}
