struct S { char pad[0x3B8]; int fl; };
/** @sly2-annot addr=0x1FC4F8
 * Purpose: vt8: 2 if looping flag 4 else 1
 * Info: proposed name GetStreamsndKind (T2); class STREAMSND; TU sound
 * Confidence: low (draft) [T2 0.45] - evidence: static analysis; sources: r7_1F8DD8+verdicts_tu_r7r8
 * See: docs/engine/sound.md#6-key-functions, docs/reference/functions/sound.md#sound-0x1faf70-0x1ff3b8
 * @end-sly2-annot */
int func_001FC4F8(struct S *p) { return (p->fl & 4) ? 2 : 1; }
