struct S { char pad[0xA0]; int a; int b; };
/** @sly2-annot addr=0x1FCF88
 * Purpose: Async callback for SUKR cmd 0xC (0x1FDBA8)
 * Info: proposed name OnSndmgrQueryC (T2); TU sound
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r7_1F8DD8
 * See: docs/reference/functions/sound.md#sound-0x1faf70-0x1ff3b8
 * @end-sly2-annot */
void func_001FCF88(int x, long l) { struct S *p = (struct S *)(int)l; p->b = x; p->a = 0; }
