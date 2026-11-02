struct S { char pad[0xA4]; int a; char pad2[0xC]; struct S *next; };
extern struct S *D_003E1060;
/** @sly2-annot addr=0x2137F0
 * Purpose: some widget in exclusive list D_003E1060 has higher priority (+0x0? field a)
 * Info: proposed name FWidgetObscured (T1); TU hud
 * Notes: names_v2 v1 proposed FUN_001aca68 (0.000)
 * Confidence: high [T1 0.85] - evidence: static naming pass; sources: r7c_hud+verdicts_followups
 * See: docs/engine/hud-ui.md#5-key-functions, docs/reference/functions/hud-ui.md#hud-0x210eb0-0x21bb88
 * @end-sly2-annot */
int FWidgetObscured(struct S *p)
{
    struct S *q;
    int unused[4];
    for (q = D_003E1060; q; q = q->next) {
        if (p->a < q->a)
            return 1;
    }
    return 0;
}
