struct VTE { short d; short pad; void (*pfn)(void *); };
struct O { int pad[3]; struct VTE *vt; char pad2[0x28 - 0x10]; unsigned long flPad : 60; unsigned int fBit60 : 1; char pad3[0xE0 - 0x30]; void *pvE0; };
struct G { int a; };
extern struct G D_003DD490;
struct P { char pad[0x24]; void *p24; char pad2[0x6C - 0x28]; struct O *po; int fActive; };
void SetTnTnsOverride(struct O *po, int n);
int FIsBasicDerivedFrom__FP5BASIC3CID(void *p, int cid);
void *PpoCur(struct G *pg);
/** @sly2-annot addr=0x1E4F20
 * Purpose: Switch the script's active tunnel (ptnCur +0x6C): release/hide old (0x221268(tn,-1), vt9),
 *   attach/show new (vt8) with owner = actor or current PO
 * Info: proposed name SetJsgTunnel (T1); TU relay
 * Confidence: mid (draft) [T1 0.55] - evidence: static analysis; sources: r6b_rest
 * See: docs/reference/functions/cutscenes.md#relay-0x1dfad8-0x1e5d80
 * @end-sly2-annot */
void SetJsgTunnel(struct P *p, struct O *po)
{
    if (p->po) {
        SetTnTnsOverride(p->po, -1);
        p->po->pvE0 = 0;
        if (p->fActive) p->po->vt[9].pfn((char *)p->po + p->po->vt[9].d);
        p->po = 0;
    }
    if (!po) return;
    p->po = po;
    p->fActive = !po->fBit60;
    if (p->fActive) po->vt[8].pfn((char *)po + po->vt[8].d);
    po->pvE0 = (p->p24 && FIsBasicDerivedFrom__FP5BASIC3CID(p->p24, 0x30)) ? p->p24 : PpoCur(&D_003DD490);
    SetTnTnsOverride(po, 1);
}
