struct S { int a, b; int x8; int xc; int pad[2]; int x18; int x1c; };
extern char D_003D9ED0[];
void func_001AF4D8(void *p, int a, int b, int c, int d);
/** @sly2-annot addr=0x1B0688
 * Purpose: Item callback wrapper of PushPauseMenuPage
 * Info: proposed name PauseItemPushPage (T2); TU menu
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r5b_rest
 * See: docs/reference/functions/hud-ui.md#menu-0x1ae928-0x1b0b78
 * @end-sly2-annot */
void func_001B0688(struct S *p)
{
    func_001AF4D8(D_003D9ED0, p->x8, p->xc, 0, p->x18);
    if (p) p->x1c |= 2;
}
