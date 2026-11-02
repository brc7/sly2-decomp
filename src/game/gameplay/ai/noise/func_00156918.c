typedef int qword __attribute__((mode(TI)));
struct Q { char pad[0x58]; int x; };
struct P { char pad[0x14]; struct Q *pq; char pad2[8]; qword v20; };
extern int FCheckLm__FP2LMf(void *p, float z);
extern float func_00156898(struct P *p);
static __inline__ float GetZ(qword q) { float z; __asm__("pextuw %0, $0, %1" : "=r"(z) : "r"(q)); return z; }
/** @sly2-annot addr=0x156918
 * Purpose: True if a position is within the noise's height range and current radius
 * Info: proposed name FPosInNoise (T2); TU noise
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r3b_rest
 * See: docs/reference/functions/guards-ai.md#noise-0x1564e8-0x157200,
 *   docs/reference/functions/guards-ai.md#stepguard-0x205e70-0x20b840
 * @end-sly2-annot */
int func_00156918(struct P *p, qword pos)
{
    qword d, l;
    float dist;
    __asm__("vsub.xyzw %0, %1, %2" : "=j"(d) : "j"(pos), "j"(p->v20));
    if (!FCheckLm__FP2LMf(&p->pq->x, GetZ(d)))
        return 0;
    __asm__("vmul.xy %0, %1, %1\n\tvaddy.x %0, %0, %0y" : "=&j"(l) : "j"(d));
    __asm__("vsqrt Q, %1x\n\tvwaitq\n\tvaddq.x %0, $vf0, Q" : "=j"(l) : "j"(l));
    __asm__("qmfc2.ni %0, %1" : "=r"(dist) : "j"(l));
    { int f = func_00156898(p) < dist; return f ^ 1; }
}
