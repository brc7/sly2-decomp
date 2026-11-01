typedef unsigned long ulong;
struct Q { char pad[0xA0]; int a0; char pad2[0x1C]; int c0; };
struct A { char pad[0x10]; struct A *next; char pad2[0xD4]; struct Q *pq; char pad3[0x5C]; ulong fl; };
struct A *PsoFindPhysRoot(struct A *p)
{
    struct A *r = 0;
    while (p) {
        r = p;
        if (p->pq && (p->pq->c0 != 3 || p->pq->a0 != 3))
            break;
        if (p->fl & (1L << 34))
            break;
        p = p->next;
    }
    return r;
}
