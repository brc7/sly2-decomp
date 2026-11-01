struct S { char pad[0x150]; int c150; int a154[4]; int c164; int a168[4]; int f178; };
int FMatchesLoName(void *, int);
int FIsBasicDerivedFrom__FP5BASIC3CID(void *, int);
int FIgnoreSoIntersection(struct S *, void *);
/** @sly2-annot addr=0x132140
 * Purpose: BARRIER vt61: match other SO against aoid[coid]/acid[ccid]; with fAllow (+0x178) matched->3 else
 *   base; without fAllow matched->2 else 1
 * Info: proposed name FIgnoreBarrierIntersection (T1); class BARRIER slot61; TU barrier
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r2b_rest+verdicts_A1+verify_side
 * See: docs/engine/gameplay-objects.md#gameplay-object-classes,
 *   docs/engine/gameplay-objects.md#key-functions,
 *   docs/reference/functions/gameplay-objects.md#barrier-0x1320c8-0x132570
 * @end-sly2-annot */
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
