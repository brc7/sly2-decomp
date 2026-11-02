typedef int qword __attribute__((mode(TI)));
/** @sly2-annot addr=0x259DA8
 * Purpose: float lane 1 (prot3w then mtc1) [game linkonce (inline/template)]
 * Info: proposed name VU_VECTOR_GetY (T2); TU game_linkonce
 * Confidence: low (draft) [T2 0.45] - evidence: static analysis; sources: sdk_names
 * See: docs/reference/functions/core-library.md#game_linkonce-0x254f00-0x259de4
 * @end-sly2-annot */
float func_00259DA8(qword *p) { float f; qword t; __asm__("prot3w %0, %1" : "=&r"(t) : "r"(*p)); __asm__("mtc1 %1, %0" : "=f"(f) : "r"(t)); return f; }
