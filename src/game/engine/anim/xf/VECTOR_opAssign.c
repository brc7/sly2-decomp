typedef int qword __attribute__((mode(TI)));
/** @sly2-annot addr=0x256D18
 * Purpose: VECTOR::operator=(VU_VECTOR): store a 128-bit VU vector into a VECTOR and return it
 * Info: proposed name VECTOR_opAssign (T1); TU game_linkonce
 * Notes: top50_round (0.60): Body *p=v;return p (12 B). Sits between __9VU_VECTORRC6VECTOR (0x256D08) and
 *   __ml__FG8VU_FLOATG9VU_VECTOR (0x256D28) in the linkonce block. Matched callers write it as an assignment
 *   into existing storage: RefOpMult/RefOp
 * Confidence: mid (draft) [T1 0.60] - evidence: semantic review; sources: sdk_names+top50_round
 * See: docs/reference/functions/core-library.md#game_linkonce-0x254f00-0x259de4
 * @end-sly2-annot */
qword *VECTOR_opAssign(qword *p, qword v) { *p = v; return p; }
