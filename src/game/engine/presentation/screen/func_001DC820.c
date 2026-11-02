struct E { int n; int a; };
struct G2 { char pad[0x4C]; struct E ae[0x40]; int nPad; int i; };
extern struct G2 D_003DA150;
struct T { int a[2]; char *pch; int b[5]; };
extern struct T D_002BCD00[];
extern char D_002A79A0[];
void func_0021ECD8(char *pch, char *fmt, char *s);
/** @sly2-annot addr=0x1DC820
 * Purpose: Text callback rows 0x442-0x447: power-up cost of row i
 * Info: proposed name BuildThiefnetPowerupCostText (T2); TU screen
 * Confidence: low (draft) [T2 0.45] - evidence: static analysis; sources: r9_bhv_screen
 * See: docs/reference/functions/hud-ui.md#screen-0x1d93d8-0x1de2a0
 * @end-sly2-annot */
void func_001DC820(char *pch, int i) {
    struct G2 *pg = &D_003DA150;
    unsigned int n = pg->ae[pg->i + i].n;
    char *s = 0;
    if (n < 0x2B) s = D_002BCD00[n].pch;
    if (s) func_0021ECD8(pch, D_002A79A0, s);
    else *pch = 0;
}
