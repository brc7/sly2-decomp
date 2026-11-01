struct E { int a[3]; int oid; char pad[0xA0]; };
struct S { char pad[0xE0]; int c; struct E *ae; };
/** @sly2-annot addr=0x1F6030
 * Purpose: Find SOPART by oid (+0x0C)
 * Info: proposed name PsopartFindSo (T1); TU so
 * Confidence: mid (draft) [T1 0.55] - evidence: static analysis; sources: r6_1D5150
 * See: docs/reference/functions/guards-ai.md#step-0x203948-0x205e70,
 *   docs/reference/functions/object-model.md#lo-0x1a5270-0x1a7518,
 *   docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8
 * @end-sly2-annot */
struct E *func_001F6030(struct S *p, int oid)
{
    int i;
    for (i = 0; i < p->c; i++) { struct E *pe = &p->ae[i]; if (pe->oid == oid) return pe; }
    return 0;
}
