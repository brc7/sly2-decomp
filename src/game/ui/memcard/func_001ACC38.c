struct H { char pad[0xFD50]; int x; };
extern struct H D_002E65E0;
struct CLOCK { float t; float dt; char pad[0x34]; float t3c; };
extern struct CLOCK D_002DDED0;
struct S { char pad[0x44]; int x44; float x48; };
/** @sly2-annot addr=0x1ACC38
 * Purpose: Memcard dialog (D_002E6550): set phase +0x44 and its start time +0x48 (g_clock t3C); phases 2/3
 *   clear g_save+0xFD50
 * Info: proposed name SetMcDialogPhase (T2); TU memcard
 * Notes: v3: superseded SetMcUiPhase | top50_round (0.45): Body. Neighbours SetMcDialogState 0x1ACB58
 *   (+0x40), CMemcardDialog_SetMode (+0x28). All callers pass D_002E6550 (memcard job/dialog code: McJob*,
 *   OnMcDialogChoice, RunNextBootStep, UpdateTitleScreen). Phase meaning unkno
 * Confidence: low (draft) [T2 0.45] - evidence: semantic review; sources: r4_17F090+top50_round
 * See: docs/reference/functions/save-memcard.md#memcard-0x1aa978-0x1ae150
 * @end-sly2-annot */
void func_001ACC38(struct S *p, int n)
{
    if (n == p->x44) return;
    switch (n) { case 2: case 3: D_002E65E0.x = 0; }
    p->x44 = n;
    p->x48 = D_002DDED0.t3c;
}
