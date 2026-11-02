typedef int qword __attribute__((mode(TI)));
extern qword D_002A2DF0;

/** @sly2-annot addr=0x11F288
 * Purpose: Component-wise minimum over c vectors (vmini, seeded with +big D_002A2DF0)
 * Info: proposed name VecMinArray (T1); TU vec
 * Confidence: mid (draft) [T1 0.55] - evidence: static analysis; sources: r1_116918
 * See: docs/reference/functions/core-library.md#vec-0x11e198-0x11f440
 * @end-sly2-annot */
qword VecMinArray(int c, qword *p)
{
    qword m = D_002A2DF0;
    int i;
    for (i = 0; i < c; i++) {
        qword v = p[i];
        __asm__("vmini.xyz %0, %1, %2" : "=j"(m) : "j"(v), "0"(m));
    }
    return m;
}
