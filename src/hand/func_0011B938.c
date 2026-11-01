// (Claude agent) Matched: MulM3q writes rows through an out pointer, copying each row right after storing it (interleaved m/t stores).
typedef int qword __attribute__((mode(TI)));
typedef struct { qword r[3]; } M3;
extern qword D_002A2DB0;
extern qword D_002A2DC0;
M3 func_0011BA08(qword a, qword b);
#define ROW(o, a0, a1, a2) __asm__ __volatile__("vmulax.xyz $ACC, %1, %0x\n\tvmadday.xyz $ACC, %2, %0y\n\tvmaddz.xyz %0, %3, %0z" : "+j"(o) : "j"(a0), "j"(a1), "j"(a2))
static inline void MulM3q(qword a0, qword a1, qword a2, M3 *pb, M3 *pd)
{
    M3 m;
    register qword r2 __asm__("vf1");
    register qword r0 __asm__("vf2");
    register qword r1 __asm__("vf3");
    r2 = pb->r[2];
    r0 = pb->r[0];
    ROW(r2, a0, a1, a2);
    r1 = pb->r[1];
    ROW(r0, a0, a1, a2);
    ROW(r1, a0, a1, a2);
    m.r[0] = r0; pd->r[0] = m.r[0];
    m.r[1] = r1; pd->r[1] = m.r[1];
    m.r[2] = r2; pd->r[2] = m.r[2];
}
/** @sly2-annot addr=0x11B938
 * Purpose: Rotation that takes the matrix's local Z onto world Z; optionally returns dmat and the uprighted
 *   matrix
 * Info: proposed name TiltMatUpright (T1); TU mat
 * Confidence: mid (draft) [T1 0.75] - evidence: static analysis; sources: r1_116918
 * See: docs/reference/functions/core-library.md#mat-0x11b300-0x11cca8,
 *   docs/reference/functions/splice-scripting.md#bif-0x11f440-0x1258f0
 * @end-sly2-annot */
void func_0011B938(M3 *pm, M3 *pmA, M3 *pmB)
{
    M3 local;
    M3 t;
    M3 *p = pmA ? pmA : &local;
    qword a0, a1, a2;
    t = func_0011BA08(pm->r[2], D_002A2DC0);
    a0 = t.r[0]; p->r[0] = a0;
    a1 = t.r[1]; p->r[1] = a1;
    a2 = t.r[2]; p->r[2] = a2;
    if (pmB) {
        MulM3q(a0, a1, a2, pm, &t);
        *pmB = t;
    }
}
