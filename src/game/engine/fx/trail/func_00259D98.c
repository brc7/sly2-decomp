typedef int qword __attribute__((mode(TI)));
/** @sly2-annot addr=0x259D98
 * Purpose: float lane 0 of qword (mtc1 low word) [game linkonce (inline/template)]
 * Info: proposed name VU_VECTOR_GetX (T2); TU game_linkonce
 * Confidence: low (draft) [T2 0.45] - evidence: static analysis; sources: sdk_names
 * See: docs/reference/functions/core-library.md#game_linkonce-0x254f00-0x259de4
 * @end-sly2-annot */
float func_00259D98(qword *p) { float f; __asm__("mtc1 %1, %0" : "=f"(f) : "r"(*p)); return f; }
