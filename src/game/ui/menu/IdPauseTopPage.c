struct E { int a; void *pv; int pad[5]; };
struct P { int pad[10]; struct E ae[1]; };
extern char D_002BFDA8[], D_002BFE08[], D_002BFE68[];
int func_001D9880(struct P *p);
/** @sly2-annot addr=0x1B0078
 * Purpose: Id of current top page (0x1E/0x1F/0x19 for the three save/load pages) or -1
 * Info: proposed name IdPauseTopPage (T1); TU menu
 * Notes: names_v2 v1 proposed InitStep__FP4STEP (0.198)
 * Confidence: high [T1 0.85] - evidence: static naming pass; sources: r5b_rest+verdicts_followups
 * See: docs/engine/hud-ui.md#5-key-functions, docs/reference/functions/hud-ui.md#menu-0x1ae928-0x1b0b78
 * @end-sly2-annot */
int IdPauseTopPage(struct P *p)
{
    int n;
    void *pv;
    if (!func_001D9880(p)) return -1;
    n = p->pad[9];
    if (n <= 0) return -1;
    pv = p->ae[n - 1].pv;
    if (pv == D_002BFDA8) return 0x1E;
    if (pv == D_002BFE08) return 0x1F;
    if (pv == D_002BFE68) return 0x19;
    return -1;
}
