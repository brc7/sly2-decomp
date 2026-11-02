struct VTE { short d; short pad; void (*pfn)(); };
struct O { int x0, x4, x8; struct VTE *vt; };
struct N { int x0; struct O *po; int x8; struct N *next; };
struct S { char pad[0x150]; struct N *pn; };
struct MSG { struct S *p; struct O *po; };
struct G;
extern struct G D_003DD490;
void func_00229738(struct O *po, struct N *pn);
void func_002296C0(struct G *pg, struct N *pn);
void HandleBasicMessage(struct S *p, int k, struct O *q);
/** @sly2-annot addr=0x229370
 * Purpose: drops XAs on some messages then base HandleLoMessage 0x133590
 * Info: proposed name HandleWaterMessage (T2); class WATER slot5; TU water
 * Notes: names_v2 C proposed TranslateSoToPosSafe__FP2SOP6VECTORT1 (0.200)
 * Confidence: low (draft) [T2 0.45] - evidence: static naming pass; sources: r8_21BB88
 * See: docs/reference/functions/physics-collision.md#water-0x228b48-0x229ef0
 * @end-sly2-annot */
void func_00229370(struct S *p, int k, struct O *q)
{
    if (k == 1 && q != (struct O *)p) {
        struct MSG msg;
        struct N **ppn = &p->pn;
        struct N *pn = *ppn;
        if (pn) {
            if (pn->po == q) {
                *ppn = pn->next;
            } else {
                for (;;) {
                    struct N *pnPrev = *ppn;
                    pn = pnPrev->next;
                    ppn = &pnPrev->next;
                    if (!pn) break;
                    if (pn->po == q) {
                        pnPrev->next = pn->next;
                        break;
                    }
                }
            }
        }
        msg.p = p;
        msg.po = pn->po;
        if (pn) {
            pn->next = 0;
            func_00229738(pn->po, pn);
            func_002296C0(&D_003DD490, pn);
        }
        msg.po->vt[4].pfn((char *)msg.po + msg.po->vt[4].d, 5, &msg);
    }
    HandleBasicMessage(p, k, q);
}
