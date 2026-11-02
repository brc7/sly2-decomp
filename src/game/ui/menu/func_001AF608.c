struct S { char pad[0x24]; int x24; char pad2[0x1C0]; int x1e8; int x1ec; };
void func_001AEAA8(struct S *p, int n);
int func_001B0108(struct S *p);
/** @sly2-annot addr=0x1AF608
 * Purpose: In state 7: state 2, pop page if it is a save page
 * Info: proposed name PopPauseMenuPage (T2); TU menu
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r5b_rest
 * See: docs/reference/functions/hud-ui.md#menu-0x1ae928-0x1b0b78
 * @end-sly2-annot */
void func_001AF608(struct S *p)
{
    if (p->x1e8 == 7) {
        func_001AEAA8(p, 2);
        if (func_001B0108(p)) p->x1ec = p->x24 = p->x24 - 1;
    }
}
