typedef int qword __attribute__((mode(TI)));
extern qword D_002A2E00;

/** @sly2-annot addr=0x11F2B8
 * Purpose: Component-wise maximum over c vectors
 * Info: proposed name VecMaxArray (T1); TU vec
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: r1_116918
 * See: docs/reference/functions/core-library.md#vec-0x11e198-0x11f440
 * @end-sly2-annot */
qword VecMaxArray(int c, qword *p)
{
    qword m = D_002A2E00;
    int i;
    for (i = 0; i < c; i++) {
        qword v = p[i];
        __asm__("vmax.xyz %0, %1, %2" : "=j"(m) : "j"(v), "0"(m));
    }
    return m;
}
