struct T { int n; };
struct S { char pad[0x60]; int n60; };
int func_001EDB80(struct S *p, struct T *t);
/** @sly2-annot addr=0x1EE420
 * Purpose: EYES vt10: SAA::FMatches or *poid == +0x60
 * Info: proposed name EYES_FMatches (T2); class EYES slot10; TU shdanim
 * Notes: names_v2 v1 proposed PsaiFromEyesShd__FP4EYESP3SHD (0.000)
 * Confidence: low (draft) [T2 0.40] - evidence: static naming pass; sources: r6b_rest
 * See: docs/reference/functions/rendering.md#shdanim-0x1ed7a8-0x1ee988
 * @end-sly2-annot */
int func_001EE420(struct S *p, struct T *t)
{
    int f = 0;
    if (func_001EDB80(p, t) || t->n == p->n60) f = 1;
    return f;
}
