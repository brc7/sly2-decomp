struct B { char pad[0x5408]; char x5408[0x44]; int x544C; int x5450; };
void func_002333B4(void *p, int c, int n);
void func_001ABB10(void *s, void *b, char *name);
int func_001ABFC0(void *s, char *name, void *p);
int func_001AC9A0(void *s, char *name, void *b, int n);
/** @sly2-annot addr=0x1AC4C8
 * Purpose: For each slot: BuildSaveFilePath, read dir entry, McReadFile
 * Info: proposed name ReadSaveSlotHeaders (T2); TU memcard
 * Notes: names_v2 v1 proposed FUN_0018d368 (0.000)
 * Confidence: low (draft) [T2 0.40] - evidence: static naming pass; sources: r4_17F090
 * See: docs/engine/save-memcard.md#5-key-functions,
 *   docs/reference/functions/save-memcard.md#memcard-0x1aa978-0x1ae150
 * @end-sly2-annot */
int func_001AC4C8(void *s, struct B *b)
{
    char name[0x50];
    int r;
    func_002333B4(b, 0, 0x5464);
    func_001ABB10(s, b, name);
    r = func_001ABFC0(s, name, b->x5408);
    if (r)
        return r;
    if ((b->x5450 & 0xD4) != 4)
        return 0x14;
    if (b->x544C != 0x5408)
        return 0x1C;
    return func_001AC9A0(s, name, b, 0x5408);
}
