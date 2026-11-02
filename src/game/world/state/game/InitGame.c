struct S { int a, b, c, d; int f10, f14, f18; };
extern char D_003D4A60[];
extern char D_002DE578[];
extern int D_002E1E00;
void *func_002333B4(void *pv, int n, int cb);
void func_001D8D80(void *p, int n);
void InitDifficulty(void *p);
/** @sly2-annot addr=0x1613F8
 * Purpose: Resets the 0x1C-byte GAME record g_game (D_002DEB30): zero, oidJob/oidTask/oidTaskPending
 *   (+0x10/+0x14/+0x18) = -1; ResetGs(g_gs,0); InitDifficulty; D_002E1E00=0
 * Info: proposed name InitGame (T1); TU game
 * Notes: superseded r3_1539A8 proposal: InitGameState (0.45)
 * Confidence: mid (draft) [T1 0.55] - evidence: static analysis; sources: r3b_rest+verdicts_tu_r3r5
 * See: docs/engine/game-progression.md#8-key-functions, docs/engine/save-memcard.md#5-key-functions,
 *   docs/reference/functions/game-progression.md#game-0x1613a8-0x166270
 * @end-sly2-annot */
void InitGame(struct S *p)
{
    func_002333B4(p, 0, sizeof(*p));
    p->f10 = -1;
    p->f14 = -1;
    p->f18 = -1;
    func_001D8D80(D_003D4A60, 0);
    InitDifficulty(D_002DE578);
    D_002E1E00 = 0;
}
