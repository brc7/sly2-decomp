struct S { int pad[0x94/4]; int a94; int a98; };
/** @sly2-annot addr=0x19E9C8
 * Purpose: Clear two fields (+0x94/+0x98) of the adjust record
 * Info: proposed name ResetJtPipeAdjust (T2); TU pipe_jt
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r4b_rest
 * See: docs/reference/functions/player.md#bentley-0x134648-0x1378d8,
 *   docs/reference/functions/player.md#jt-0x188488-0x18e540,
 *   docs/reference/functions/player.md#pipe_jt-0x19de10-0x19f0a8
 * @end-sly2-annot */
void func_0019E9C8(void *p, struct S *s) { s->a94 = 0; s->a98 = 0; }
