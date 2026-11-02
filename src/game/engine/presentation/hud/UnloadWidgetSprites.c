struct C { char pad[0x44]; int a; int b; };
struct S { char pad[0x48]; int x48; int x4c; int x50; int x54; char pad2[0x28]; short x80; short pad3; int x84; char pad4[0x38]; struct C *pc; char pad5[0x34]; int xf8; char xfc[4]; };
void ClearSl(void *p);
/** @sly2-annot addr=0x212780
 * Purpose: state 0 entry: drop sprite tables and lists
 * Info: proposed name UnloadWidgetSprites (T1); TU hud
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: r7c_hud
 * See: docs/reference/functions/hud-ui.md#hud-0x210eb0-0x21bb88
 * @end-sly2-annot */
void UnloadWidgetSprites(struct S *p)
{
    struct C *pc;
    p->xf8 = 0;
    p->x50 = 0;
    p->x54 = 0;
    p->x80 = 0;
    p->x84 = 0;
    ClearSl(p->xfc);
    pc = p->pc;
    pc->a = 0;
    pc->b = 0;
}
