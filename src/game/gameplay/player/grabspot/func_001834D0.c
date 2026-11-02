typedef int qword __attribute__((mode(TI)));
struct Q { char pad[0x218]; qword *p218; };
struct P { char pad[0x10C]; float f10C; };
qword func_001A62E8(void *p);
float atan2f(float y, float x);
static __inline__ float GetX(qword q) { float x; __asm__("mtc1 %1,%0" : "=f"(x) : "r"(q)); return x; }
static __inline__ float GetY(qword q) { float y; __asm__("prot3w %0,%1" : "=r"(y) : "r"(q)); return y; }
/** @sly2-annot addr=0x1834D0
 * Purpose: Angle (atan2) from the pole axis to the player
 * Info: proposed name GetCid34ClosestParam (T2); class CID34; TU grabspot
 * Confidence: low (draft) [T2 0.35] - evidence: static analysis; sources: r4b_rest
 * See: docs/reference/functions/player.md#grabspot-0x182c70-0x1848a8
 * @end-sly2-annot */
void func_001834D0(struct P *p, struct Q *q, float *pg)
{
    qword a = *q->p218;
    qword b = func_001A62E8(p);
    qword d;
    __asm__("vsub.xyzw %0, %1, %2" : "=j"(d) : "j"(a), "j"(b));
    *pg = atan2f(GetY(d), GetX(d)) * p->f10C;
}
