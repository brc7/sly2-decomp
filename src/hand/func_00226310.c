struct T { char pad[0x3C]; int k; };
struct S { char pad[0x1C0]; struct T *t; };
void *FIgnoreSoIntersection(struct S *p);
void *func_00134310(void *v, int a);
/** @sly2-annot addr=0x226310
 * Purpose: base filter 0x1F4FC8; add flag when other matches own target kind
 * Info: proposed name FFilterTurret (T2); class TURRET slot61; TU turret
 * Confidence: low (draft) [T2 0.35] - evidence: static analysis; sources: r8_21BB88
 * See: docs/reference/functions/gameplay-objects.md#turret-0x2259d8-0x226550
 * @end-sly2-annot */
void *func_00226310(struct S *p, struct T *q)
{
    void *v = FIgnoreSoIntersection(p);
    if (p->t && q->k == p->t->k) v = func_00134310(v, 1);
    return v;
}
