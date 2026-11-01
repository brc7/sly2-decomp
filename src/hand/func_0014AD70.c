struct E { int k; int a, b; };
struct Q { char pad[0x58]; struct E *ae; };
struct P { char pad[0x38]; struct Q *q; int i; int c; };
void func_0014AE38(struct P *p, int n);
/** @sly2-annot addr=0x14AD70
 * Purpose: advance to next route point and act on its kind
 * Info: proposed name AdvanceBhv182RoutePoint (T2); TU tu_14A008
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r2b_rest
 * See: docs/reference/functions/guards-ai.md#tu_14a008-0x14a008-0x14b660
 * @end-sly2-annot */
void func_0014AD70(struct P *p)
{
    int k;
    if (p->i < p->c) {
        p->i++;
        k = p->q->ae[p->i].k;
    } else {
        k = p->q->ae[p->i].k;
        p->i--;
    }
    switch (k) {
    case 0:
        func_0014AE38(p, 1);
        break;
    case 1:
        func_0014AE38(p, 2);
        break;
    }
}
