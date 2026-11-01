typedef int qword __attribute__((mode(TI)));
float func_002458F0(float y, float x);
void func_00204660(void *p, float g);
static __inline__ float GetX(qword q) { float x; __asm__("mtc1 %1, %0" : "=f"(x) : "r"(q)); return x; }
static __inline__ float GetY(qword q) { float y; __asm__("prot3w %0, %1" : "=r"(y) : "r"(q)); return y; }
/** @sly2-annot addr=0x2046B0
 * Purpose: SetStepRadTarget(atan2(v.y, v.x)) for a direction vector
 * Info: proposed name SetStepRadTargetVec (T1); TU step
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: r7b_guard
 * See: docs/reference/functions/guards-ai.md#step-0x203948-0x205e70
 * @end-sly2-annot */
void func_002046B0(void *p, qword v) { func_00204660(p, func_002458F0(GetY(v), GetX(v))); }
