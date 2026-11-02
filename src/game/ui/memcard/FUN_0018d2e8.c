struct B { int a[20]; };
void BuildSaveFilePath(void *a, void *b, struct B *pb);
void func_001AC900(void *a, struct B *pb, void *b, int n);
/** @sly2-annot addr=0x1AC420
 * Purpose: BuildSaveFilePath + McWriteFile
 * Info: proposed name WriteSaveSlotFile (EXISTING); TU memcard
 * Notes: agent/verifier proposes WriteSaveSlotFile for this existing name (see existing_name_changes.csv) |
 *   arbitration: draft replacement (not re-derived) | RENAME proposed -> WriteSaveSlotFile
 *   (existing_name_changes.csv)
 * Confidence: high [EXISTING 0.40] - evidence: arbitration; pre-existing symbol in symbol_addrs.txt; sources:
 *   r4_17F090+arbitration
 * See: docs/engine/save-memcard.md#5-key-functions,
 *   docs/reference/functions/save-memcard.md#memcard-0x1aa978-0x1ae150
 * @end-sly2-annot */
void FUN_0018d2e8(void *a, void *b)
{
    struct B buf;
    BuildSaveFilePath(a, b, &buf);
    func_001AC900(a, &buf, b, 0x5408);
}
