typedef int qword __attribute__((mode(TI)));
struct S { int pad[0x30/4]; qword a; qword b; };
/** @sly2-annot addr=0x19EAA0
 * Purpose: Store two vectors in the pipe context
 * Info: proposed name SetJtPipeVectors (T2); TU pipe_jt
 * Confidence: low (draft) [T2 0.25] - evidence: static analysis; sources: r4b_rest
 * See: docs/reference/functions/player.md#pipe_jt-0x19de10-0x19f0a8
 * @end-sly2-annot */
void func_0019EAA0(struct S *s, qword a, qword b) { s->a = a; s->b = b; }
