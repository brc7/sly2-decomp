typedef int qword __attribute__((mode(TI)));
struct VT { struct { short d; short pad; void *(*pfn)(void *); } a[8]; };
struct O { char pad[0x20]; struct VT *pvt; };
extern qword func_001A62E8(void *p);
extern qword func_0011E208(float a, float b, float c);
extern float func_0011D448(float a, float b);
extern float D_002BE8B0;
extern qword D_002A2DC0;
static __inline__ qword VAdd(qword a, qword b) { qword r; __asm__("vadd.xyzw %0, %1, %2" : "=j"(r) : "j"(a), "j"(b)); return r; }
/** @sly2-annot addr=0x182D78
 * Purpose: Default: random point in a sphere around the owner (GRandInRange pan/tilt/radius), zero dv
 * Info: proposed name GetGrabspotSparklePos (T2); class GRABSPOT; TU grabspot
 * Confidence: low (draft) [T2 0.45] - evidence: static analysis; sources: r4b_rest
 * See: docs/reference/functions/player.md#grabspot-0x182c70-0x1848a8
 * @end-sly2-annot */
void func_00182D78(struct O *p, qword *pv, qword *pdv)
{
    void *po = p->pvt->a[6].pfn((char *)p + p->pvt->a[6].d);
    float gPan = func_0011D448(-3.14159265f, 3.14159265f);
    float t = func_0011D448(-1.0f, 1.0f);
    float gTilt = __builtin_fabsf(t) * t * 1.5707964f;
    float gR = func_0011D448(0.0f, D_002BE8B0);
    *pv = VAdd(func_001A62E8(po), func_0011E208(gPan, gTilt, gR));
    *pdv = D_002A2DC0;
}
