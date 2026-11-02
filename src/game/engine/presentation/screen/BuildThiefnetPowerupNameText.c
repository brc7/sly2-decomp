struct E { int n; int a; };
struct G2 { char pad[0x4C]; struct E ae[0x40]; int nPad; int i; };
extern struct G2 D_003DA150;
struct T { int a[5]; int id; int b[2]; };
extern struct T D_002BCD00[];
extern struct G D_003DD490;
void *PchzFindSwLangstr__FP2SWi(struct G *g, int n);
void strcpy(char *p, void *q);
/** @sly2-annot addr=0x1DC790
 * Purpose: Text callback rows 0x43C-0x441: power-up name of row i
 * Info: proposed name BuildThiefnetPowerupNameText (T1); TU screen
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: r9_bhv_screen
 * See: docs/engine/minigames.md#8-safehouse-and-thiefnet-ccode,
 *   docs/reference/functions/hud-ui.md#screen-0x1d93d8-0x1de2a0
 * @end-sly2-annot */
void BuildThiefnetPowerupNameText(char *pch, int i) {
    struct G2 *pg = &D_003DA150;
    unsigned int n = pg->ae[pg->i + i].n;
    int id = -1;
    if (n < 0x2B) id = D_002BCD00[n].id;
    if (id != -1) strcpy(pch, PchzFindSwLangstr__FP2SWi(&D_003DD490, id));
    else *pch = 0;
}
