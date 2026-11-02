struct S { char pad[0x4E]; short c; };
void func_001FB658(struct S *, int, int);
/** @sly2-annot addr=0x1FB018
 * Purpose: Refcounted pause: ++cPause==1 -> SetSndFlags(6,2) (request pause)
 * Info: proposed name PauseSnd (T1); TU sound
 * Confidence: mid (draft) [T1 0.70] - evidence: static analysis; sources: r7_1F8DD8
 * See: docs/reference/functions/sound.md#sound-0x1faf70-0x1ff3b8
 * @end-sly2-annot */
void func_001FB018(struct S *p) { if (++p->c == 1) func_001FB658(p, 6, 2); }
