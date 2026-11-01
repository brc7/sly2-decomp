struct Q { int a; char *ab; };
struct A { char pad[0x50]; int c; };
struct Q *PsmdFindSm(struct A *p, int n);
int CsmtDistanceSm(struct A *p, int n, int y, int x)
{
    struct Q *pq = PsmdFindSm(p, n);
    int r = 0;
    if (pq)
        r = pq->ab[y * p->c + x];
    return r;
}
