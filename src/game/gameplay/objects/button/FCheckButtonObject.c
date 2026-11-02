struct P { char pad[0x2AC]; int c; int a[8]; int c2; int a2[8]; };
int FIsBasicDerivedFrom__FP5BASIC3CID(int x, int y);
/** @sly2-annot addr=0x149100
 * Purpose: Is the object allowed to push (push oid/cid lists vs no-push lists)
 * Info: proposed name FCheckButtonObject (T1); TU button
 * Confidence: mid (draft) [T1 0.65] - evidence: static analysis; sources: r2b_rest
 * See: docs/reference/functions/gameplay-objects.md#button-0x148620-0x149a50
 * @end-sly2-annot */
int FCheckButtonObject(struct P *p, int x)
{
    int i, j;
    if (p->c > 0) {
        for (i = 0; i < p->c; i++)
            if (p->a[i] == x) break;
        if (i >= p->c) return 0;
    }
    if (p->c2 > 0) {
        for (j = 0; j < p->c2; j++)
            if (FIsBasicDerivedFrom__FP5BASIC3CID(x, p->a2[j])) break;
        if (j >= p->c2) return 0;
    }
    return 1;
}
