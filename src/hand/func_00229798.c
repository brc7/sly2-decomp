struct VTE { short d; short pad; void (*pf)(void *, void *); };
struct O { int a[3]; struct VTE *vt; };
struct N { struct O *po; int a; struct N *next; };
struct S { char pad[0x138]; struct N *first; };
/** @sly2-annot addr=0x229798
 * Purpose: for each XA call owner vt71 (AddSoExternalAccelerations)
 * Info: proposed name AddSoXaAccelerations (T2); TU water
 * Confidence: low (draft) [T2 0.35] - evidence: static analysis; sources: r8_21BB88
 * See: docs/reference/functions/physics-collision.md#solve-0x1f8dd8-0x1faf70,
 *   docs/reference/functions/physics-collision.md#water-0x228b48-0x229ef0
 * @end-sly2-annot */
void func_00229798(struct S *p)
{
    struct N *n;
    for (n = p->first; n; n = n->next) {
        struct O *po = n->po;
        po->vt[71].pf((char *)po + po->vt[71].d, n);
    }
}
