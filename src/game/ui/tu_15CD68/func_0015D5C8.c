extern int *D_002BE1B8[];
extern int *D_002BE080[];
struct S { char pad[0x110]; int n110; int pad114[2]; int n11C; int n120; int pad124; int n128; };
/** @sly2-annot addr=0x15D5C8
 * Purpose: Label id for the widget's current mode/sub-mode from tables D_002BE080/D_002BE1B8 (-1 if none)
 * Info: proposed name OidWidget131Label (T2); TU tu_15CD68
 * Confidence: low (draft) [T2 0.35] - evidence: static analysis; sources: r3b_rest
 * See: docs/reference/functions/hud-ui.md#tu_15cd68-0x15cd68-0x15dc80
 * @end-sly2-annot */
int func_0015D5C8(struct S *p)
{
    int k = p->n11C;
    if (k == -1)
        return -1;
    if (k == 8 && p->n120 != -1)
        return D_002BE1B8[p->n128][p->n120];
    return D_002BE080[p->n110][p->n11C];
}
