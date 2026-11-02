struct VTE { short d; short pad; void (*pf)(void *, void *); };
struct O { int a[3]; struct VTE *vt; };
struct B { int a; struct O *po; };
/** @sly2-annot addr=0x2292D8
 * Purpose: po->vt72(p)
 * Info: class WATER slot71; TU water
 * Notes: descriptive only: water: call other vt72 (contact) | no usable identifier; purpose only
 * Confidence: low (draft) [T2 0.25] - evidence: static analysis; sources: r8_21BB88
 * @end-sly2-annot */
void func_002292D8(void *p, struct B *b)
{
    struct O *po = b->po;
    po->vt[72].pf((char *)po + po->vt[72].d, p);
}
