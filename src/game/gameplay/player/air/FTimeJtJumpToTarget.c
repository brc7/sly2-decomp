typedef int qword __attribute__((mode(TI)));
static __inline__ float GetZ(qword q) { float z; __asm__("pextuw %0, $0, %1" : "=r"(z) : "r"(q)); return z; }
struct C { char pad[0x88]; float g88; };
struct B { char pad[0xE8]; struct C *pc; };
struct S { struct B *pb; };
int CSolveQuadratic__FfffPf(float, float, float, float *);
/** @sly2-annot addr=0x19A400
 * Purpose: Solve 0.5 g t^2 + dvz t + dz = 0 (CSolveQuadratic) for the landing time; fail if no positive root
 * Info: proposed name FTimeJtJumpToTarget (T1); TU air
 * Confidence: mid (draft) [T1 0.60] - evidence: static analysis; sources: r4b_rest
 * See: docs/reference/functions/player.md#air-0x195950-0x19cbf8
 * @end-sly2-annot */
int FTimeJtJumpToTarget(struct S *p, qword a, qword b, qword c, qword d, float *pg) {
    float ag[2];
    int n = CSolveQuadratic__FfffPf(p->pb->pc->g88 * 0.5f, GetZ(b) - GetZ(d), GetZ(a) - GetZ(c), ag);
    if (n <= 0) return 0;
    {
        float g = ag[n - 1];
        if (g <= 0.0f) return 0;
        *pg = g;
    }
    return 1;
}
