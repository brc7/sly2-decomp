struct VTE { short d; short pad; float *(*pfn)(void *); };
struct S { char pad[0xC]; struct VTE *vt; char pad1[0x1D8 - 0x10]; float rad; };
float func_002458F0(float y, float x);
float func_0011CF60(float r);
/** @sly2-annot addr=0x1B1868
 * Purpose: True when facing within 1 degree of stored heading
 * Info: proposed name FCid96FacingHeading (T2); class CID96; TU tu_1B0B78
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r5b_rest
 * See: docs/reference/functions/gameplay-objects.md#tu_1b0b78-0x1b0b78-0x1b1db8
 * @end-sly2-annot */
int func_001B1868(struct S *p)
{
    float *a = p->vt[31].pfn((char *)p + p->vt[31].d);
    float *b = p->vt[31].pfn((char *)p + p->vt[31].d);
    float r = func_0011CF60(p->rad - func_002458F0(a[1], b[0]));
    if (__builtin_fabsf(r) < 0.017453294f)
        return 1;
    return 0;
}
