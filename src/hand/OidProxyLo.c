struct Q { int x0; struct Q *next; int x8; int xc; void *x10; };
struct S { char pad[0x3C]; struct Q *pq; };
/** @sly2-annot addr=0x1A6260
 * Purpose: Walk ppxr list (x3C) while node owner==plo, return last oid or -1
 * Info: proposed name OidProxyLo (T1); TU lo
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: r4_17F090
 * See: docs/reference/functions/object-model.md#lo-0x1a5270-0x1a7518
 * @end-sly2-annot */
int OidProxyLo(struct S *p)
{
    int r = -1;
    struct Q *q = p->pq;
    while (q) {
        if (q->x10 != p) break;
        r = q->x8;
        q = q->next;
    }
    return r;
}
