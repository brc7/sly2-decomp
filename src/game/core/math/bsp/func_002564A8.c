typedef int qword __attribute__((mode(TI)));
/** @sly2-annot addr=0x2564A8
 * Purpose: *p = qword v; return p [game linkonce (inline/template)]
 * Info: proposed name VECTOR4_opAssign (T2); TU game_linkonce
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: sdk_names
 * See: docs/reference/functions/core-library.md#game_linkonce-0x254f00-0x259de4
 * @end-sly2-annot */
qword *func_002564A8(qword *p, qword v) { *p = v; return p; }
