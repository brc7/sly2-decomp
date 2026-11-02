struct VTE { short d; short pad; void (*pfn)(void *, int, int); };
struct O { int pad[3]; struct VTE *vt; };
struct P { char pad[0xA4]; int c; struct O *apo[4]; int aB8[4]; int aC8[4]; };
int FIsBasicDerivedFrom__FP5BASIC3CID(void *p, int cid);
/** @sly2-annot addr=0x1E51A0
 * Purpose: Hold a STEP (max 4, +0xA4 cstepHold/apstepHold/anHold5/anHold6) and call its vt83(a,b)
 * Info: proposed name AddJsgStepHold (T1); TU relay
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: r6b_rest
 * See: docs/reference/functions/cutscenes.md#relay-0x1dfad8-0x1e5d80
 * @end-sly2-annot */
void AddJsgStepHold(struct P *p, struct O *po, int a, int b)
{
    int i;
    if (!FIsBasicDerivedFrom__FP5BASIC3CID(po, 0x33)) return;
    for (i = 0; i < p->c; i++)
        if (p->apo[i] == po) return;
    if ((unsigned)p->c >= 4) return;
    p->aB8[p->c] = a;
    p->aC8[p->c] = b;
    p->apo[p->c] = po;
    p->c++;
    po->vt[0x53].pfn((char *)po + po->vt[0x53].d, a, b);
}
