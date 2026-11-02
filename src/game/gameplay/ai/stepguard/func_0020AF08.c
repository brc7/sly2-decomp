struct S { char pad[0x68]; float g; };
/** @sly2-annot addr=0x20AF08
 * Purpose: +0x68 = deg*pi/180
 * Info: proposed name SetDegAngle68 (T2); TU stepguard
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r7b_guard
 * See: docs/reference/functions/guards-ai.md#stepguard-0x205e70-0x20b840
 * @end-sly2-annot */
void func_0020AF08(struct S *p, float g) { p->g = g * 0.01745329424738884f; }
