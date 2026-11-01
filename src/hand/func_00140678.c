struct X { char pad[0x10DC]; int b; int pad2; int a; int d; int c; };
void func_001431D0(struct X *p, int a, int b, int c, int d);
/** @sly2-annot addr=0x140678
 * Purpose: vt95: store interact kind/target/mode/fight target (+0x10E8..+0x10F0); if target changed release
 *   the old target's mark (0x1431D0)
 * Info: proposed name SetStepplayerPlayerInteractMode (T1); class STEPPLAYER slot95; TU stepplayer
 * Confidence: mid (draft) [T1 0.75] - evidence: static analysis; sources: r2b_rest
 * See: docs/reference/functions/player.md#stepplayer-0x13ff70-0x144638
 * @end-sly2-annot */
void func_00140678(struct X *p, int a, int b, int c, int d)
{
    if (b != p->b) func_001431D0(p, a, b, c, d);
    p->b = b;
    p->a = a;
    p->d = d;
    p->c = c;
}
