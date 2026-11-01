struct Q { int x0; struct Q *next; int x8; int xc; void *x10; };
struct S { char pad[0x3C]; struct Q *pq; };
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
