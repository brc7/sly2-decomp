struct Q { int x0; struct Q *next; int x8; int xc; void *x10; };
struct S { char pad[0x18]; int x18; char pad2[0x20]; struct Q *pq; };
int FMatchesLoName(struct S *p, int b)
{
    struct Q *q;
    if (p->x18 == b) return 1;
    for (q = p->pq; q && q->x10 == p; q = q->next)
        if (q->x8 == b) return 1;
    return 0;
}
