struct S { char pad[0x54]; int f54; int f58; int f5c; char p2[0xB8-0x60]; int fb8; int fbc; int fc0; char p3[0xF0-0xC4]; int ff0; int ff4; };
struct G { char pad[0x3AFC]; int c; };
extern struct G D_003DD490;
void func_001A5270(struct S *p);
/** @sly2-annot addr=0x1635A0
 * Purpose: TASK (cid 0x85) Init: LO Init, ids -1, counts tasks in g_sw+0x3AFC
 * Info: proposed name InitTask (T1); class TASK slot7; TU game
 * Notes: superseded r3_1539A8 proposal: InitCid85 (0.75)
 * Confidence: mid (draft) [T1 0.70] - evidence: static analysis; sources: r3b_rest
 * See: docs/engine/game-progression.md#8-key-functions,
 *   docs/reference/functions/game-progression.md#game-0x1613a8-0x166270
 * @end-sly2-annot */
void func_001635A0(struct S *p)
{
    func_001A5270(p);
    p->f54 = -1;
    p->ff0 = -1;
    p->f5c = -1;
    p->fb8 = -1;
    p->fbc = -1;
    p->fc0 = -1;
    p->ff4 = 0;
    D_003DD490.c++;
}
