struct R { int a[4]; R(int c, int f, void **ap); };
struct N { int x0; N *pnext; };
struct T { int x0; void **ap; };
struct S { char pad[0xE0]; T *xE0; int pad2; N *xE8; };
extern "C" {
int IcopyFromPspare__FP5PROXYP10PROXYSPARE(S *p, N *pn);
R func_001CB360(S *p);
}
/** @sly2-annot addr=0x1CB360
 * Purpose: return CRef list (tag 0x2002) of objects of all held spares
 * Info: proposed name RefProxyCopies (T2); TU proxy
 * Notes: names_v2 C proposed PostSmLoadCallback__FP2SM5MSGIDPv (0.420)
 * Confidence: low (draft) [T2 0.35] - evidence: static naming pass; sources: r5_1AE130
 * See: docs/reference/functions/object-model.md#proxy-0x1c9940-0x1cb908
 * @end-sly2-annot */
R func_001CB360(S *p)
{
    void *ap[0x20];
    unsigned int c = 0;
    N *pn;
    for (pn = p->xE8; pn && c < 0x20; pn = pn->pnext) {
        int i = IcopyFromPspare__FP5PROXYP10PROXYSPARE(p, pn);
        ap[c++] = p->xE0->ap[i];
    }
    return R(c, 0x2002, ap);
}
