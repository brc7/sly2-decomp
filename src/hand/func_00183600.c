typedef int qword __attribute__((mode(TI)));
struct O { char pad[0x10C]; float g10C; };
extern qword func_001A62E8(void *p);
extern qword SetVectorCylind(float a, float b, float c);
extern float GRandInRange__Fff(float a, float b);
extern float D_002BE8C4;
extern qword D_002A2DC0;
static __inline__ qword VAdd(qword a, qword b) { qword r; __asm__("vadd.xyzw %0, %1, %2" : "=j"(r) : "j"(a), "j"(b)); return r; }
/** @sly2-annot addr=0x183600
 * Purpose: Random point on a ring around the pole
 * Info: proposed name GetCid34SparklePos (T2); class CID34; TU grabspot
 * Confidence: low (draft) [T2 0.35] - evidence: static analysis; sources: r4b_rest
 * See: docs/reference/functions/player.md#grabspot-0x182c70-0x1848a8
 * @end-sly2-annot */
void func_00183600(struct O *p, qword *pv, qword *pdv)
{
    float gPan = GRandInRange__Fff(-3.14159265f, 3.14159265f);
    float gR = GRandInRange__Fff(0.0f, D_002BE8C4) + p->g10C;
    *pv = VAdd(func_001A62E8(p), SetVectorCylind(gPan, gR, 0.0f));
    *pdv = D_002A2DC0;
}
