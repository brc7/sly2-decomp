typedef struct {
    char pad[0x10];
    int x10;
    int pad14;
    int x18;
    float x1C;
    int x20;
    float x24;
} OBJ;
void func_001DA578(OBJ *, int);
void func_001DA5C8(OBJ *, int);
/** @sly2-annot addr=0x1DA4B8
 * Purpose: Request a notice text n for duration g (queue behind the current one); n=-1 cancels
 * Info: proposed name PostNotice (T1); TU screen
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: r9_bhv_screen
 * See: docs/reference/functions/game-progression.md#game-0x1613a8-0x166270,
 *   docs/reference/functions/hud-ui.md#screen-0x1d93d8-0x1de2a0
 * @end-sly2-annot */
void func_001DA4B8(OBJ *p, int n, float g)
{
    if (n == -1) {
        func_001DA578(p, -1);
        return;
    }
    switch (p->x10) {
    case 1:
        func_001DA5C8(p, -1);
    case 0:
    case 3:
        p->x18 = n;
        p->x1C = g;
        func_001DA5C8(p, 1);
        break;
    case 2:
        if (p->x18 == n) {
            p->x1C = g;
            break;
        }
    case 4:
        p->x20 = n;
        p->x24 = g;
        break;
    }
}
