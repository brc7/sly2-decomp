typedef int qword __attribute__((mode(TI)));
typedef struct VECTOR { float x, y, z, w; } __attribute__((aligned(16))) VECTOR;
struct S { char pad[0x240]; VECTOR v; };
void EvaluateShapeS(void *a, float g, VECTOR *pv, int n);
/** @sly2-annot addr=0x1CBB58
 * Purpose: GRABSPOT iface: position at distance (+ offset from given point)
 * Info: proposed name GetRailGrabPos (T2); class RAIL; TU rail
 * Notes: names_v2 C proposed CloneHbsk__FP4HBSKT0 (0.638)
 * Confidence: low (draft) [T2 0.30] - evidence: static naming pass; sources: r5_1AE130
 * See: docs/reference/functions/player.md#rail-0x1cb908-0x1cbeb0
 * @end-sly2-annot */
void func_001CBB58(void *a, float g, struct S *p, VECTOR *pv)
{
    VECTOR v;
    qword diff;
    EvaluateShapeS(a, g, &v, 0);
    if (pv) {
        __asm__("vsub.xyzw %0, %1, %2" : "=j"(diff) : "j"(*(qword *)&v), "j"(*(qword *)&p->v));
        *(qword *)pv = diff;
    }
}
