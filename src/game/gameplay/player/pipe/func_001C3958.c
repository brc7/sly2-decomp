typedef int qword __attribute__((mode(TI)));
struct S { char pad[0xE4]; int fE4; };
qword func_001A62E8(void *q);
void FindShapeClosestPoint(struct S *p, qword q, int a, void *pv, int b, int c, float *pf);
float SMaxShape(struct S *p);
static inline float GLimit(float x, float lo, float hi)
{
    if (x < lo) return lo;
    if (hi < x) return hi;
    return x;
}
/** @sly2-annot addr=0x1C3958
 * Purpose: GRABSPOT iface: closest distance clamped
 * Info: proposed name PIPE_ClampGrabDistance (T2); class PIPE; TU pipe
 * Notes: names_v2 v1 proposed GetJtDiapi__FP2JTP6DIALOGP5DIAPI (0.438)
 * Confidence: low (draft) [T2 0.35] - evidence: static naming pass; sources: r5_1AE130
 * See: docs/reference/functions/player.md#pipe-0x1c3390-0x1c3d70
 * @end-sly2-annot */
void func_001C3958(struct S *p, void *q, float *pf)
{
    FindShapeClosestPoint(p, func_001A62E8(q), 0, 0, 0, 0, pf);
    if (p->fE4 == 0) {
        *pf = GLimit(*pf, 100.0f, SMaxShape(p) - 35.0f);
    }
}
