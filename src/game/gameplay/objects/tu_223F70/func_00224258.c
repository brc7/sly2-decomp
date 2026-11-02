struct VTE { short d; short pad; void (*pf)(); };
struct O { int a[3]; struct VTE *vt; char pad[0x14C]; int k; };
struct Q { char pad[0x18]; int k; };
void SetLoParent_1A58D0(struct O *p, struct Q *q, int a);
/** @sly2-annot addr=0x224258
 * Purpose: states 0x1A/0x29 -> SetState(3); else SetLoParent(q or 0,3) and SetState(2)
 * Info: TU tu_223F70
 * Notes: descriptive only: Cid100 attach/detach to carrier | no usable identifier; purpose only
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r8_21BB88
 * See: docs/reference/functions/gameplay-objects.md#tu_223f70-0x223f70-0x2259d8
 * @end-sly2-annot */
void func_00224258(struct O *p, struct Q *q)
{
    int k = p->k;
    if (k == 0x1A || k == 0x29) p->vt[76].pf((char *)p + p->vt[76].d, 3);
    else {
        SetLoParent_1A58D0(p, q->k == 1 ? 0 : q, 3);
        p->vt[76].pf((char *)p + p->vt[76].d, 2);
    }
}
