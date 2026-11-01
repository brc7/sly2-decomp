struct S { char pad[0x80]; int a; float b; int *pa; float *pb; };
/** @sly2-annot addr=0x22C818
 * Purpose: b=1.0, a=0, pa=&a, pb=&b
 * Info: proposed name InitXfValueRef (T2); TU xf
 * Confidence: low (draft) [T2 0.25] - evidence: static analysis; sources: r8_21BB88
 * See: docs/engine/animation.md#6-source-files, docs/reference/functions/animation.md#xf-0x22c818-0x231100,
 *   docs/reference/tu-map.md#tu-map-address-range---source-file
 * @end-sly2-annot */
void func_0022C818(void *q, struct S *p)
{
    p->b = 1.0f;
    p->a = 0;
    p->pa = &p->a;
    p->pb = &p->b;
}
