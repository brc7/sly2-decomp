typedef int qword __attribute__((mode(TI)));
struct S { char pad[0x10]; void *p10; char pad2[0x1C]; void *p30; };
qword func_001A62E8(void *p);
void func_001710F8(void *p, qword q, int a, int b, int c);
/** @sly2-annot addr=0x171AF8
 * Purpose: GetCid71TargetPose relative to the guard position
 * Info: proposed name GetBhv169TargetPose (T2); TU tu_1709A8
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r3b_rest
 * See: docs/reference/functions/guards-ai.md#tu_1709a8-0x1709a8-0x171f58
 * @end-sly2-annot */
void func_00171AF8(struct S *p, int a, int b, int c) { func_001710F8(p->p30, func_001A62E8(p->p10), a, b, c); }
