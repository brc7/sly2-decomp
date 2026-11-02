struct S { char pad[0xFD50]; void *x; };
extern char D_002E6550[];
void *McJobGetInfo(struct S *s);
void *CreateSaveDir(struct S *s);
void *func_001AC420(struct S *s, void *x);
void func_001ACC38(void *p, int n);
/** @sly2-annot addr=0x1ABB60
 * Purpose: Thread job: card info check, CreateSaveDir, write one slot file (0x1AC420)
 * Info: proposed name McJobSaveSlot (T1); TU memcard
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r4_17F090+verdicts_tu_r4r6
 * See: docs/engine/save-memcard.md#2-memory-card-memcardcpp-0x1aa978-0x1ae130,
 *   docs/engine/save-memcard.md#5-key-functions,
 *   docs/reference/functions/save-memcard.md#memcard-0x1aa978-0x1ae150
 * @end-sly2-annot */
void *McJobSaveSlot(struct S *s)
{
    void *p = McJobGetInfo(s); if (!p) p = CreateSaveDir(s); if (!p) p = func_001AC420(s, s->x); if (p) func_001ACC38(D_002E6550, 3); return p;
}
