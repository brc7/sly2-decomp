struct S { char pad[0xB0]; int fb0; int fb4; void *fb8; void *fbc; void *fc0; int fc4; char pad2[0xCC - 0xC8]; void *fcc; };
void func_001A5E28(struct S *p);
int func_002121E8(void *p, int n);
void func_00162F60(struct S *p);
void func_00162FD0(struct S *p);
void func_00163170(struct S *p, int n);
/** @sly2-annot addr=0x163078
 * Purpose: JOB Update: completion sequence phases 1 (intro label on widget) -> 2/3 -> done, advancing when
 *   the widget label finishes
 * Info: proposed name UpdateJob (T1); class JOB slot16; TU game
 * Notes: superseded r3_1539A8 proposal: UpdateCid84 (0.65) | runtime6 G7/G8: job phase 1 banner (S61)
 * Confidence: mid (draft) [T1 0.65] - evidence: runtime (save-state RAM); sources: r3b_rest+runtime
 * See: docs/engine/game-progression.md#job-lo-child-0x100, docs/engine/game-progression.md#8-key-functions,
 *   docs/reference/functions/game-progression.md#game-0x1613a8-0x166270
 * @end-sly2-annot */
void func_00163078(struct S *p)
{
    func_001A5E28(p);
    switch (p->fc4) {
    case 1:
        if (!func_002121E8(p->fb8, p->fb0)) {
            func_00162F60(p);
            if (p->fc0 || p->fcc)
                func_00163170(p, 3);
            else
                func_00163170(p, 2);
        }
        break;
    case 2:
        if (!func_002121E8(p->fb8, p->fb4)) {
            func_00162FD0(p);
            func_00163170(p, 0);
        }
        break;
    case 3:
        if (!func_002121E8(p->fbc, 0x30F))
            func_00162FD0(p);
        break;
    }
}
