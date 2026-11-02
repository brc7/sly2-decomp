struct S { char pad[0x4C]; short c; };
void func_00258348(struct S *, int);
void func_001FCD10(struct S *);
/** @sly2-annot addr=0x1FCCA0
 * Purpose: vt2: --cref==0 -> dtor + free to 0x3C0 pool
 * Info: proposed name DecrefStreamsnd (T1); class STREAMSND; TU sound
 * Confidence: mid (draft) [T1 0.70] - evidence: static analysis; sources: r7_1F8DD8
 * See: docs/reference/functions/sound.md#sound-0x1faf70-0x1ff3b8
 * @end-sly2-annot */
void func_001FCCA0(struct S *p) { if (--p->c == 0 && p) { func_00258348(p, 0); func_001FCD10(p); } }
