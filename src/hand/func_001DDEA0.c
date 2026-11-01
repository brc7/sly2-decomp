struct O { char pad[0x6C]; int n; };
struct E { struct O *po; int a, b; };
struct G2 { char pad[0x48]; int c; struct E ae[1]; };
extern struct G2 D_003DA3C0;
extern struct G D_003DD490;
void *func_0013E368(struct G *g, int n);
void func_0021ED58(void *p, void *q);
/** @sly2-annot addr=0x1DDEA0
 * Purpose: Text callback rows 0x51E-0x527: job name of row i
 * Info: proposed name BuildJobListRowText (T1); TU screen
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: r9_bhv_screen
 * See: docs/reference/functions/hud-ui.md#screen-0x1d93d8-0x1de2a0
 * @end-sly2-annot */
void func_001DDEA0(void *p, int i) {
    struct G2 *pg = &D_003DA3C0;
    if (i < pg->c) func_0021ED58(p, func_0013E368(&D_003DD490, pg->ae[i].po->n));
}
