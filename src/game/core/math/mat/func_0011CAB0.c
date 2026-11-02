typedef int qword __attribute__((mode(TI)));
typedef struct { qword r[3]; } M3;
static inline float sqrtf_(float x) { float r; __asm__("sqrt.s %0, %1" : "=&f"(r) : "f"(x)); return r; }
static inline float Dot(qword v)
{
    qword d, one;
    float g;
    __asm__("vmul.xyz %0, %2, %2\n\tvaddw.x %1, $vf0, $vf0w\n\tvadday.x $ACC, %0, %0y\n\tvmaddz.x %0, %1, %0z" : "=j"(d), "=j"(one) : "j"(v));
    __asm__("mtc1 %1, %0" : "=f"(g) : "r"(d));
    return g;
}
/** @sly2-annot addr=0x11CAB0
 * Purpose: Largest row length (max of the three row dot products, then sqrt)
 * Info: proposed name SMaxRowMatrix3 (T2); TU mat
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r1_116918
 * See: docs/reference/functions/core-library.md#mat-0x11b300-0x11cca8
 * @end-sly2-annot */
float func_0011CAB0(M3 *pm)
{
    float d0 = Dot(pm->r[0]);
    float d1 = Dot(pm->r[1]);
    float d2 = Dot(pm->r[2]);
    float g;
    if (d1 < d0 && d2 < d0)
        g = d0;
    else if (d2 < d1)
        g = d1;
    else
        g = d2;
    return sqrtf_(g);
}
