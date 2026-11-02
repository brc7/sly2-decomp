typedef int qword __attribute__((mode(TI)));
struct O { char pad[0x110]; qword pos; };
extern qword ConvertAloPos(void *p, void *q, qword v);
extern qword SetVectorCylind(float a, float b, float c);
extern float GRandInRange__Fff(float a, float b);
extern float D_002BE90C;
extern qword D_002A2DC0;
static __inline__ qword VAdd(qword a, qword b) { qword r; __asm__("vadd.xyzw %0, %1, %2" : "=j"(r) : "j"(a), "j"(b)); return r; }
/** @sly2-annot addr=0x1845A0
 * Purpose: Random point on a disc around its point
 * Info: proposed name GetCid27SparklePos (T2); class CID27; TU grabspot
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r4b_rest+verdicts_B
 * See: docs/reference/functions/player.md#grabspot-0x182c70-0x1848a8
 * @end-sly2-annot */
void func_001845A0(struct O *p, qword *pv, qword *pdv)
{
    float gPan = GRandInRange__Fff(-3.14159265f, 3.14159265f);
    float gR = GRandInRange__Fff(0.0f, D_002BE90C);
    *pv = VAdd(ConvertAloPos(p, 0, p->pos), SetVectorCylind(gPan, gR, 0.0f));
    *pdv = D_002A2DC0;
}
