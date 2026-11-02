struct R { int n0; void *p4; void *p8; int nC; };
struct S { char pad[0x194]; void *p194; int a198[1]; };
void ReleaseSma(void *p);
void ReleaseAsega(void *pp);
void ApplyAsegCur__FP4ASEGP3ALOffiPP5ASEGA(void *paseg, struct S *p, float t, float svt, int grf, void *pp);
void ReplaceSma(void *a, struct S *p, int b, int c, void *d);
/** @sly2-annot addr=0x17F4C0
 * Purpose: Retract current ASEGA (x194) / SM (x198) and apply the record's clip and SM state
 * Info: proposed name StartCid47Anim (T2); class CID47; TU tu_17F090
 * Notes: names_v2 v1 proposed HandleStepguardGrfsgsc__FP9STEPGUARD (0.052)
 * Confidence: low (draft) [T2 0.40] - evidence: static naming pass; sources: r4b_rest
 * See: docs/reference/functions/gameplay-objects.md#tu_17f090-0x17f090-0x182c70
 * @end-sly2-annot */
void func_0017F4C0(struct S *p, struct R *r) {
    ReleaseSma(p->a198);
    ReleaseAsega(&p->p194);
    if (r) {
        ApplyAsegCur__FP4ASEGP3ALOffiPP5ASEGA(r->p4, p, 0.0f, 1.0f, r->nC, &p->p194);
        ReplaceSma(r->p8, p, -1, r->nC, p->a198);
    }
}
