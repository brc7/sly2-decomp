struct S { char pad[0x150]; int c150; int a154[4]; int c164; int a168[4]; int f178; };
int FMatchesLoName(void *, int);
int FIsBasicDerivedFrom__FP5BASIC3CID(void *, int);
int FIgnoreSoIntersection(struct S *, void *);
int FIgnoreBarrierIntersection(struct S *p, void *pv) {
    int i, j;
    int f;
    i = 0;
    f = 0;
    for (; i < p->c150; i++) {
        if (FMatchesLoName(pv, p->a154[i])) f = 1;
    }
    for (j = 0; j < p->c164; j++) {
        if (FIsBasicDerivedFrom__FP5BASIC3CID(pv, p->a168[j])) f = 1;
    }
    {
        int r;
        if (p->f178) {
            if (f) r = 3;
            else r = FIgnoreSoIntersection(p, pv);
        } else r = f ? 2 : 1;
        return r;
    }
}
