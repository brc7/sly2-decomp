struct T { int a, b, c; };
struct P { char pad[0x74]; struct T *pt; };
struct CB { int cb; int n; };
struct T *func_001AE1D0(struct CB *);
/** @sly2-annot addr=0x1F0128
 * Purpose: Allocates the 12-byte {oid, a, b} record at +0x74 consumed by PostSmLoad
 * Info: proposed name SetSmPendingApply (T1); TU sm
 * Confidence: mid (draft) [T1 0.65] - evidence: semantic review; sources: semantic_tu_docs/sm
 * See: docs/reference/functions/animation.md#sm-0x1efa90-0x1f1768
 * @end-sly2-annot */
void func_001F0128(struct P *p, int a, int b, int c)
{
    struct CB cb;
    struct T *pt;
    cb.cb = 0xC; cb.n = 0x10;
    pt = func_001AE1D0(&cb);
    p->pt = pt;
    pt->a = a; pt->b = b; pt->c = c;
}
