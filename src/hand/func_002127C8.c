struct Q { char pad[0x58]; int k; int pad2; int a; int b; };
struct S { char pad[0x40]; struct Q *q; char pad2[8]; struct S *next; };
extern struct S *D_003E106C;
extern int D_002BE538;
/** @sly2-annot addr=0x2127C8
 * Purpose: true when no deferred widget load (list D_003E106C) is younger than 2 frames
 * Info: proposed name FWidgetLoadsIdle (T2); TU hud
 * Notes: names_v2 v1 proposed FUN_001ab600 (0.000)
 * Confidence: low (draft) [T2 0.35] - evidence: static naming pass; sources: r7c_hud
 * See: docs/reference/functions/hud-ui.md#hud-0x210eb0-0x21bb88
 * @end-sly2-annot */
int func_002127C8(void)
{
    struct S *p;
    for (p = D_003E106C; p; p = p->next) {
        struct Q *q = p->q;
        int v;
        if (q->k == 0)
            continue;
        if (q->k == 1)
            v = D_002BE538 - q->a;
        else
            v = q->b;
        if (v < 2)
            return 0;
    }
    return 1;
}
