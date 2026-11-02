struct EC { char pad[0x70]; unsigned long fl; };
struct O { int pad[4]; struct O *pNext; int n14; char pad2[0xEC - 0x18]; struct EC *pec; };
struct A { int pad[5]; int n14; };
struct B { int pad; struct O *po; int pad2[2]; unsigned long fl; };
int FIsBasicDerivedFrom__FP5BASIC3CID(struct O *po, int n);
/** @sly2-annot addr=0x154568
 * Purpose: Hit-test filter callback: rejects flagged objects, same-group objects and anything under a cid
 *   0x33 ancestor
 * Info: TU detector
 * Notes: no usable identifier; purpose only
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r3_1539A8
 * @end-sly2-annot */
int func_00154568(struct A *a, struct B *b)
{
    struct O *po;
    if (b->fl & 0x1000000000000L)
        return 0;
    po = b->po;
    if (po->pec->fl & 4)
        return 0;
    if (po->n14 == a->n14)
        return 0;
    {
        struct O *p = po;
        while (p && !FIsBasicDerivedFrom__FP5BASIC3CID(p, 0x33))
            p = p->pNext;
        return p == 0;
    }
}
