struct S { char pad[0x30]; float a; float b; };
/** @sly2-annot addr=0x1FB4F0
 * Purpose: Set full/stop distance (stop forced > full+1)
 * Info: proposed name SetSndRange (T1); TU sound
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: r7_1F8DD8
 * See: docs/reference/functions/sound.md#sound-0x1faf70-0x1ff3b8
 * @end-sly2-annot */
void func_001FB4F0(struct S *p, float a, float b) { if (b <= a) { b = a + 1.0f; p->b = b; } else p->b = b; p->a = a; }
