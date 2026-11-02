struct E { int n; int a; };
struct G2 { char pad[0x4C]; struct E ae[0x40]; int nPad; int i; };
extern struct G2 D_003DA150;
struct T { char *pch; int b[2]; };
extern struct T D_002C3288[];
extern char D_002A79A0[];
void sprintf(char *pch, char *fmt, char *s);
/** @sly2-annot addr=0x1DC9C0
 * Purpose: Text callback rows 0x44E-0x453: loot price of row i
 * Info: proposed name BuildThiefnetLootPriceText (T2); TU screen
 * Confidence: low (draft) [T2 0.45] - evidence: static analysis; sources: r9_bhv_screen
 * See: docs/reference/functions/hud-ui.md#screen-0x1d93d8-0x1de2a0
 * @end-sly2-annot */
void func_001DC9C0(char *pch, int i) {
    struct G2 *pg = &D_003DA150;
    unsigned int n = pg->ae[pg->i + i].n;
    char *s = 0;
    if (n < 0x40) s = D_002C3288[n].pch;
    if (s) sprintf(pch, D_002A79A0, s);
    else *pch = 0;
}
