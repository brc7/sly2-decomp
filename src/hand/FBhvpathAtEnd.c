typedef int qword __attribute__((mode(TI)));
struct O {
    char pad[0x10];
    void *p10;
    char pad2[0x3C - 0x14];
    int f3C;
    char pad3[0x64 - 0x40];
    float f64;
    char pad4[0xB0 - 0x68];
    qword vB0;
};
qword func_001A62E8(void *);
static inline float Dist(qword a, qword b) {
    qword d;
    float f;
    __asm__("vsub.xyzw %0, %2, %1" : "=&j"(d) : "j"(b), "j"(a));
    __asm__("vmul.xy %0, %1, %1\n\tvaddy.x %0, %0, %0y" : "=j"(d) : "j"(d));
    __asm__("vsqrt $Q, %1x\n\tvwaitq\n\tvaddq.x %0, $vf0, $Q" : "=j"(d) : "j"(d));
    __asm__("qmfc2.ni %0, %1" : "=r"(f) : "j"(d));
    return f;
}
/** @sly2-annot addr=0x178D00
 * Purpose: Path end is defined (+0x3C) and the guard is within +0x64 of the end point +0xB0
 * Info: proposed name FBhvpathAtEnd (T1); TU guardbhv
 * Confidence: mid (draft) [T1 0.55] - evidence: static analysis; sources: r9_bhv_screen
 * See: docs/reference/functions/guards-ai.md#guardbhv-0x174898-0x17f090,
 *   docs/reference/functions/player.md#tu_144b28-0x144b28-0x1471c0
 * @end-sly2-annot */
int FBhvpathAtEnd(struct O *p)
{
    if (!p->f3C)
        return 0;
    return Dist(func_001A62E8(p->p10), p->vB0) <= p->f64;
}
