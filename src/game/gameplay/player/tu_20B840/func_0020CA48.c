int func_0020CA98(char *p);
void ApplyAsegCur__FP4ASEGP3ALOffiPP5ASEGA(int a, int b, int c, void *pv, float g, float h);
/** @sly2-annot addr=0x20CA48
 * Purpose: clear anim job if active
 * Info: proposed name StopDirAnim (T2); TU tu_20B840
 * Notes: T2 forced: verifier: coined class word without descriptor
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r7c_hud
 * See: docs/reference/functions/animation.md#tu_20b840-0x20b840-0x20ce28
 * @end-sly2-annot */
void func_0020CA48(char *p) { if (!func_0020CA98(p)) ApplyAsegCur__FP4ASEGP3ALOffiPP5ASEGA(0, 0, 0, p + 0x24, 0.0f, 1.0f); }
