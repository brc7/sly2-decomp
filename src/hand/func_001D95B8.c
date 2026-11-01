struct VTE { short d; short pad; void (*pfn)(); };
struct O { int a, b, c; struct VTE *vt; void *p10; };
struct S { struct O *pNew; int a, b; struct O *pCur; };
extern char D_002DFC00[];
/** @sly2-annot addr=0x1D95B8
 * Purpose: If the requested top screen (list head) differs from the current one, notify the old one (vt) and
 *   switch focus
 * Info: proposed name UpdateScreenFocus (T2); TU screen
 * Confidence: low (draft) [T2 0.35] - evidence: static analysis; sources: r6b_rest
 * See: docs/reference/functions/hud-ui.md#screen-0x1d93d8-0x1de2a0
 * @end-sly2-annot */
void func_001D95B8(struct S *p) {
    struct O *po = p->pNew;
    struct O *pOld;
    if (po == p->pCur) return;
    if (p->pCur) {
        p->pCur->p10 = 0;
        {
            struct O *pc = p->pCur;
            struct VTE *e = &pc->vt[9];
            ((void (*)(void *, int, struct O *))e->pfn)((char *)pc + e->d, 0, po);
        }
    }
    pOld = p->pCur;
    p->pCur = po;
    if (po) {
        struct VTE *e = &po->vt[9];
        po->p10 = D_002DFC00;
        ((void (*)(void *, int, struct O *))e->pfn)((char *)po + e->d, 1, pOld);
    }
}
