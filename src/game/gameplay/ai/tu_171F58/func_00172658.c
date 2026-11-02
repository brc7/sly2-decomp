typedef int qword __attribute__((mode(TI)));
struct O { char pad[0xE74]; struct O *pE74; int nE78; char pad2[0xFE8 - 0xE7C]; int nFE8; };
struct S { char pad[0x10]; struct O *p10; };
qword func_001A62E8(struct O *p);
qword func_00172760(struct S *p);
static inline float Dist(qword a, qword b) {
    qword d;
    float f;
    __asm__("vsub.xyzw %0, %1, %2" : "=j"(d) : "j"(a), "j"(b));
    __asm__("vmul.xy %0, %1, %1
	vaddy.x %0, %0, %0y" : "=j"(d) : "j"(d));
    __asm__("vsqrt $Q, %1x\n\tvwaitq\n\tvaddq.x %0, $vf0, $Q" : "=j"(d) : "j"(d));
    __asm__("qmfc2.ni %0, %1" : "=r"(f) : "j"(d));
    return f;
}
/** @sly2-annot addr=0x172658
 * Purpose: True if the guard can't search or the last-known position is nearer than the target object + 50
 * Info: proposed name FBhv171TargetReached (T2); TU tu_171F58
 * Confidence: low (draft) [T2 0.35] - evidence: static analysis; sources: r3b_rest
 * See: docs/reference/functions/guards-ai.md#tu_171f58-0x171f58-0x173158
 * @end-sly2-annot */
int func_00172658(struct S *p) {
    struct O *o = p->p10;
    struct O *t;
    float d;
    if (!(o->nFE8 & 2) || o->nE78 == 2) return 1;
    t = o->pE74;
    if (!t) return 0;
    d = Dist(func_001A62E8(t), func_001A62E8(p->p10));
    if (Dist(func_00172760(p), func_001A62E8(p->p10)) < d + 50.0f) return 1;
    return 0;
}
