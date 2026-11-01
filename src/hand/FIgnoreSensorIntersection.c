typedef struct {
    char pad[0x150];
    int c1;
    void *a1[7];
    int c2;
    void *a2[8];
} OBJ;
int FMatchesLoName(void *, void *);
int FIsBasicDerivedFrom__FP5BASIC3CID(void *, void *);
int func_00134310(int, int);
/** @sly2-annot addr=0x1DE2D8
 * Purpose: SENSOR vt61 (SO FIgnoreSoIntersection override): ignore objects matching the no-trigger oid list
 *   (+0x150, max 7, FMatchesLoName) or class list (+0x170, FIsBasicDerivedFrom); result combined with
 *   NMaxIgnore
 * Info: proposed name FIgnoreSensorIntersection (T1); class SENSOR slot61; TU sensor
 * Confidence: mid (draft) [T1 0.55] - evidence: static analysis; sources: r6b_rest
 * See: docs/reference/functions/gameplay-objects.md#sensor-0x1de2a0-0x1dfad8
 * @end-sly2-annot */
int FIgnoreSensorIntersection(OBJ *p, void *x)
{
    int i, j;
    int r = 1;
    for (i = 0; i < p->c1; i++) {
        if (FMatchesLoName(x, p->a1[i]))
            r = func_00134310(r, 2);
    }
    for (j = 0; j < p->c2; j++) {
        if (FIsBasicDerivedFrom__FP5BASIC3CID(x, p->a2[j]))
            r = func_00134310(r, 2);
    }
    return r;
}
