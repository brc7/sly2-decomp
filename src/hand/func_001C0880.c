typedef int qword __attribute__((mode(TI)));
struct F { qword *pq; int d; };
struct E { qword pos; int c; struct F *af; };
struct A { int c; struct E *ae; };
static inline int Dist(qword a, qword b)
{
    qword d;
    register int f __asm__("$2");
    __asm__("vsub.xyzw %0, %1, %2" : "=j"(d) : "j"(a), "j"(b));
    __asm__("vmul.xy %0, %1, %1" : "=j"(d) : "j"(d));
    __asm__("vaddy.x %0, %1, %1y" : "=j"(d) : "j"(d));
    __asm__("vsqrt $Q, %1x\n\tvwaitq\n\tvaddq.x %0, $vf0, $Q" : "=j"(d) : "j"(d));
    __asm__("qmfc2.ni %0, %1" : "=r"(f) : "j"(d));
    return f;
}
/** @sly2-annot addr=0x1C0880
 * Purpose: Compute edge costs (XY distance between vertices)
 * Info: proposed name HookupCg (T1); TU path
 * Notes: names_v2 v1 proposed AdjustJtDz__FP2JTiP2DZif (0.000)
 * Confidence: mid (draft) [T1 0.70] - evidence: static naming pass; sources: r5b_rest
 * See: docs/reference/functions/physics-collision.md#path-0x1bf110-0x1c2920
 * @end-sly2-annot */
void func_001C0880(struct A *pa)
{
    int i, j;
    for (i = 0; i < pa->c; i++) {
        struct E *pe = &pa->ae[i];
        for (j = 0; j < pe->c; j++) {
            register struct F *field __asm__("$3") = pe->af;
            field += j;
            field->d = Dist(pe->pos, *field->pq);
        }
    }
}
