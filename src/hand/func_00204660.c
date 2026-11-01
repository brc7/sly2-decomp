typedef int qword __attribute__((mode(TI)));
typedef struct MATRIX3 { qword a[3]; } MATRIX3;
extern qword D_002A2DC0;
MATRIX3 func_0011B500(qword v, float g);
struct S { char pad[0x1A8]; float g; int pad2; MATRIX3 m; };
/** @sly2-annot addr=0x204660
 * Purpose: Set desired heading radTarget (+0x1A8) and rebuild matTarget (+0x1B0) = LoadRotateMatrixRad(Z,
 *   rad)
 * Info: proposed name SetStepRadTarget (T1); TU step
 * Confidence: mid (draft) [T1 0.60] - evidence: static analysis; sources: r7b_guard
 * See: docs/reference/functions/core-library.md#game_linkonce-0x254f00-0x259de4,
 *   docs/reference/functions/guards-ai.md#step-0x203948-0x205e70
 * @end-sly2-annot */
void func_00204660(struct S *p, float g) { p->g = g; p->m = func_0011B500(D_002A2DC0, g); }
