typedef int qword __attribute__((mode(TI)));
struct LO { char pad[0x218]; qword *pq; };
int FIsBasicDerivedFrom__FP5BASIC3CID(struct LO *p, int n);
qword func_001C1EC0(void *p, qword q);
/** @sly2-annot addr=0x1C1FA0
 * Purpose: Object within radius of the endpoint (class 0x40 always true)
 * Info: proposed name FNearPathlinkEndpoint (T2); TU path
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r5b_rest
 * See: docs/reference/functions/physics-collision.md#path-0x1bf110-0x1c2920
 * @end-sly2-annot */
int func_001C1FA0(void *p, struct LO *plo, float r)
{
    qword a, b, d;
    float f;
    if (FIsBasicDerivedFrom__FP5BASIC3CID(plo, 0x40)) return 1;
    a = *plo->pq;
    b = func_001C1EC0(p, a);
    __asm__("vsub.xyzw %0, %1, %2\n\tvmul.xy %0, %0, %0\n\tvaddy.x %0, %0, %0y\n\tvsqrt $Q, %0x\n\tvwaitq\n\tvaddq.x %0, $vf0, $Q" : "=j"(d) : "j"(a), "j"(b));
    __asm__("qmfc2.ni %0, %1" : "=r"(f) : "j"(d));
    return f <= r;
}
