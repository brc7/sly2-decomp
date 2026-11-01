struct VTE { short d; short p; void (*pfn)(void *); };
struct VT { struct VTE a[10]; };
struct X { char pad[0xC]; struct VT *pvt; };
struct Q { char pad[0x16C]; void *p16C; };
struct P { char pad[0x3F4]; struct X *px3F4; struct X *px3F8; int n3FC; float g400; char pad2[0x494 - 0x404]; struct Q *pq; };
struct CLOCK { float t; };
extern struct CLOCK D_002DDED0;
extern int D_002C3738[];
void func_001F0730(void *, int);
#define VF(q, n) (q)->pvt->a[n].pfn((char *)(q) + (q)->pvt->a[n].d)
/** @sly2-annot addr=0x1F17F8
 * Purpose: Change pickpocket display state k (-1..2): record time, hide (vt9) or show (vt8) the two glint
 *   objects +0x3F4/+0x3F8, and drive the pouch's SMA (+0x494->+0x16C) to state oid D_002C3738[k+1] via
 *   SetSmaGoalIfReachable
 * Info: proposed name SetStepguardPickpocketState (T1); TU pickpocket
 * Notes: runtime: pickpocket pouch is CID99 (SO child), not a PO
 * Confidence: mid (draft) [T1 0.55] - evidence: runtime (save-state RAM); sources: r6b_rest+runtime-corrected
 * See: docs/reference/functions/guards-ai.md#pickpocket-0x1f1768-0x1f2108
 * @end-sly2-annot */
void func_001F17F8(struct P *p, int k)
{
    void *pv;
    if (k == p->n3FC) return;
    p->n3FC = k;
    p->g400 = D_002DDED0.t;
    switch (k) {
    case -1: case 0:
        if (p->px3F4) VF(p->px3F4, 9);
        if (p->px3F8) VF(p->px3F8, 9);
        break;
    case 1: case 2:
        if (p->px3F4) VF(p->px3F4, 8);
        if (p->px3F8) VF(p->px3F8, 8);
        break;
    }
    pv = p->pq->p16C;
    if (pv) func_001F0730(pv, D_002C3738[p->n3FC + 1]);
}
