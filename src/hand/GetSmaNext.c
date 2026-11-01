struct E { int a; int b; };
struct T { char pad[0x54]; struct E *ae; };
struct S { char pad[0x24]; struct T *pt; char pad2[0xC]; int x34; int x38; };
/** @sly2-annot addr=0x1F0808
 * Purpose: OID of ismsNext (+0x38), else of ismsCur
 * Info: proposed name GetSmaNext (T1); TU sm
 * Confidence: mid (draft) [T1 0.75] - evidence: semantic review; sources: semantic_tu_docs/sm
 * See: docs/reference/functions/animation.md#sm-0x1efa90-0x1f1768
 * @end-sly2-annot */
int GetSmaNext(struct S *p)
{
    int i = p->x38;
    if (i >= 0) return p->pt->ae[i].a;
    i = p->x34;
    if (i >= 0) return p->pt->ae[i].a;
    return -1;
}
