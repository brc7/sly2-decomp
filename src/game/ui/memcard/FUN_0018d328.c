struct B { char pad[0x5408]; char x5408[0x44]; int x544C; int x5450; };
void func_001D8D80(struct B *p, int n);
void BuildSaveFilePath(void *s, void *b, char *name);
int func_001AC900(void *s, char *name, void *b, int n);
/** @sly2-annot addr=0x1AC468
 * Purpose: ResetGs then BuildSaveFilePath + McWriteFile
 * Info: proposed name WriteSaveSlotFileReset (EXISTING); TU memcard
 * Notes: agent/verifier proposes WriteSaveSlotFileReset for this existing name (see
 *   existing_name_changes.csv) | arbitration: draft replacement (not re-derived) | RENAME proposed ->
 *   WriteSaveSlotFileReset (existing_name_changes.csv)
 * Confidence: high [EXISTING 0.35] - evidence: arbitration; pre-existing symbol in symbol_addrs.txt; sources:
 *   r4_17F090+arbitration
 * See: docs/engine/save-memcard.md#5-key-functions,
 *   docs/reference/functions/save-memcard.md#memcard-0x1aa978-0x1ae150
 * @end-sly2-annot */
int FUN_0018d328(void *s, struct B *b)
{
    char name[0x50];
    func_001D8D80(b, 0);
    BuildSaveFilePath(s, b, name);
    return func_001AC900(s, name, b, 0x5408);
}
