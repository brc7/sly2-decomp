extern int D_002C35B4;
extern int D_003DA570;
struct S { char p0[0x48]; int n48; char p1[0x250-0x4C]; int n250; };
/** @sly2-annot addr=0x1DBBA0
 * Purpose: Index past the last visible row (scroll window D_002C35B4 rows, adjusted for scroll arrows)
 * Info: proposed name IThiefnetRowEnd (T2); TU screen
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r9_bhv_screen
 * See: docs/reference/functions/hud-ui.md#screen-0x1d93d8-0x1de2a0
 * @end-sly2-annot */
int func_001DBBA0(struct S *p) {
    int n = p->n250 + D_002C35B4;
    if (D_003DA570) {
        n -= (p->n250 > 0);
        n -= (n < p->n48);
    }
    return n;
}
