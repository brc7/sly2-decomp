struct S { char pad[0xFD58]; void *x; };
extern char D_002E6550[];
void *func_001AC598(struct S *s);
void *func_001AB7D0(struct S *s);
void *func_001AC468(struct S *s, void *x);
void func_001ACC38(void *p, int n);
/** @sly2-annot addr=0x1ABC68
 * Purpose: Thread job: card check, CreateSaveDir, write via 0x1AC468 (with ResetGs)
 * Info: proposed name McJobWriteSave (T2); TU memcard
 * Notes: names_v2 v1 proposed FUN_0018c7f8__FP4SAVE (0.000)
 * Confidence: low (draft) [T2 0.45] - evidence: static naming pass; sources: r4_17F090
 * See: docs/engine/save-memcard.md#2-memory-card-memcardcpp-0x1aa978-0x1ae130,
 *   docs/reference/functions/save-memcard.md#memcard-0x1aa978-0x1ae150
 * @end-sly2-annot */
void *func_001ABC68(struct S *s)
{
    void *p = func_001AC598(s); if (!p) p = func_001AB7D0(s); if (!p) p = func_001AC468(s, s->x); if (p) func_001ACC38(D_002E6550, 3); return p;
}
