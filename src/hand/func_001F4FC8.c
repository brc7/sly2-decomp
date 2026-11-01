typedef unsigned long ulong;
struct VTE { short d; short p; int (*pfn)(void *); };
struct VT { struct VTE a[63]; };
struct X { char pad[0xC]; struct VT *pvt; char pad2[0x148 - 0x10]; ulong fl; };
#define VF(p) (p)->pvt->a[62].pfn((char *)(p) + (p)->pvt->a[62].d)
/** @sly2-annot addr=0x1F4FC8
 * Purpose: SO vt61: true if (pa flag +0x148 bit26 or pa fixed) and pb fixed
 * Info: proposed name FIgnoreSoIntersection (T1); class SO slot61; TU so
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r6_1D5150+verdicts_tu_r4r6
 * See: docs/engine/physics-collision.md#7-key-functions,
 *   docs/reference/functions/gameplay-objects.md#turret-0x2259d8-0x226550,
 *   docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8
 * @end-sly2-annot */
int func_001F4FC8(struct X *pa, struct X *pb)
{
    if (((pa->fl & 0x4000000) || VF(pa)) && VF(pb)) return 1;
    return 0;
}
