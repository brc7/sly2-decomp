struct S { int pad[3]; int c; float g; };
void EvaluateShapeS(int, int, float, int);
/** @sly2-annot addr=0x19E9D8
 * Purpose: Evaluate the pipe curve at the current s into out
 * Info: proposed name GetJtPipePos (T2); TU pipe_jt
 * Confidence: low (draft) [T2 0.35] - evidence: static analysis; sources: r4b_rest
 * See: docs/reference/functions/player.md#pipe_jt-0x19de10-0x19f0a8
 * @end-sly2-annot */
void func_0019E9D8(struct S *s, int x) { EvaluateShapeS(s->c, x, s->g, 0); }
