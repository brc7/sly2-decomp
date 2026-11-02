struct S { char pad[0x68]; int fl; };
/** @sly2-annot addr=0x1FE5B8
 * Purpose: Set/clear bits in SNDMGR flags
 * Info: proposed name SetSndmgrFlag (T2); TU sound
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r7_1F8DD8
 * See: docs/reference/functions/cutscenes.md#tu_1471c0-0x1471c0-0x147e20,
 *   docs/reference/functions/sound.md#sound-0x1faf70-0x1ff3b8
 * @end-sly2-annot */
void func_001FE5B8(struct S *p, int f, int fSet) { if (fSet) p->fl |= f; else p->fl &= ~f; }
