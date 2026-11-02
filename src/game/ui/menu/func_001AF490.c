struct S { char pad[0x24]; int x24; char pad2[0x1C0]; int x1e8; int x1ec; };
void SetPauseMenuState(struct S *p, int n);
/** @sly2-annot addr=0x1AF490
 * Purpose: In state 7: state 3 and select previous page
 * Info: proposed name PausePageBack (T2); TU menu
 * Confidence: low (draft) [T2 0.35] - evidence: static analysis; sources: r5b_rest
 * See: docs/reference/functions/hud-ui.md#menu-0x1ae928-0x1b0b78
 * @end-sly2-annot */
void func_001AF490(struct S *p)
{
    if (p->x1e8 == 7) {
        SetPauseMenuState(p, 3);
        p->x1ec = p->x24 - 1;
    }
}
