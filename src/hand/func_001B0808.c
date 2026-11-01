extern char D_002DEB30[];
extern void *D_003E0F9C;
extern char D_003DA3C0[];
void func_001619A0(void *p, int n, void *q);
void func_001DD300(void *p, int f);
/** @sly2-annot addr=0x1B0808
 * Purpose: Game event 0x2B; 0x1DD300(screen,flag)
 * Info: proposed name PauseItemMap (T2); TU menu
 * Confidence: low (draft) [T2 0.25] - evidence: static analysis; sources: r5b_rest
 * See: docs/reference/functions/hud-ui.md#menu-0x1ae928-0x1b0b78
 * @end-sly2-annot */
void func_001B0808(void)
{
    func_001619A0(D_002DEB30, 0x2B, 0);
    func_001DD300(D_003DA3C0, D_003E0F9C != 0);
}
