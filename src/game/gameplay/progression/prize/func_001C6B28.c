struct VTE { short d; short pad; void (*pf)(); };
struct LO { char pad[0xC]; struct VTE *vt; char pad2[0x150 - 0x10]; int x150; char pad3[0x230 - 0x154]; void *x230; };
void *func_001FBB90(int n);
void func_001FB2F8(void *p, int n);
void RequestTaskState(void *p, int n, struct LO *plo);
void SetDprizeDprizes(struct LO *p, int n);
/** @sly2-annot addr=0x1C6B28
 * Purpose: CID 92 setter (sound on collect)
 * Info: proposed name SetKeyDprizes (T2); class KEY slot76; TU prize
 * Notes: class identity from whole_game WE names/object strings: SetDprize92Dprizes -> SetKeyDprizes
 * Confidence: low (draft) [T2 0.35] - evidence: static analysis; sources:
 *   r5_1AE130+verdicts_tu_r3r5+whole_game(class identity)
 * See: docs/engine/game-progression.md#8-key-functions,
 *   docs/reference/functions/game-progression.md#prize-0x1c4e50-0x1c8cc0
 * @end-sly2-annot */
void func_001C6B28(struct LO *p, int n)
{
    if (p->x150 == n) return;
    if (n == 1) {
        func_001FB2F8(func_001FBB90(0), 0);
        if (p->x230) RequestTaskState(p->x230, 2, p);
        n = 2;
        ((void (*)(void *, int, int, int, int, int))p->vt[3].pf)((char *)p + p->vt[3].d, 5, 0, 0, 0, 0);
    }
    SetDprizeDprizes(p, n);
}
