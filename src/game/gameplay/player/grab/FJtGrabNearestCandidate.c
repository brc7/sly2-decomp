struct O { char pad[0x10C]; int x10c; };
struct E { struct O *x0; int x4; float x8; int pad[9]; };
struct G { char pad[0x4678]; int c; int pad2; struct E ae[1]; };
extern struct G D_003DD490;
extern float D_002BF660;
extern int D_002E1E40;
struct P { int x0; };
int FIsBasicDerivedFrom__FP5BASIC3CID(struct O *o, int n);
void SetJtGrabState(struct P *p, int n, struct O *o, int a);
/** @sly2-annot addr=0x1938C0
 * Purpose: Walk the sorted g_sw grab-candidate array (dist < D_002BF660); map class to grab state (HND
 *   0x13/PIPE? 0xB -> 6 for Sly g_pjt else 5; CID27 0x1B -> 8; CID12/CID34 -> 0; CID38 -> its field; CID35 ->
 *   7) and SetJtGrabState on the first match; return 1 if grabbed
 * Info: proposed name FJtGrabNearestCandidate (T1); TU grab
 * Notes: names_v2 B proposed MatchJtXmgRail__FP2JTP3XMGP6ACTADJ (0.919) | runtime2: CID27->8, CID12/34->0,
 *   rail->3
 * Confidence: mid (draft) [T1 0.55] - evidence: runtime (save-state RAM); static naming pass; sources:
 *   r4b_rest+runtime
 * See: docs/reference/functions/player.md#grab-0x191090-0x195950
 * @end-sly2-annot */
int FJtGrabNearestCandidate(struct P *p)
{
    struct G *g = &D_003DD490;
    int n = g->c;
    int i;
    for (i = 0; i < n; i++) {
        struct E *e = &g->ae[i];
        struct O *o;
        int k;
        if (e->x8 > D_002BF660) break;
        o = e->x0;
        if (FIsBasicDerivedFrom__FP5BASIC3CID(o, 0x13) || FIsBasicDerivedFrom__FP5BASIC3CID(o, 0xB))
            k = (p->x0 == D_002E1E40) ? 6 : 5;
        else if (FIsBasicDerivedFrom__FP5BASIC3CID(o, 0x1B))
            k = 8;
        else if (FIsBasicDerivedFrom__FP5BASIC3CID(o, 0xC))
            k = 0;
        else if (FIsBasicDerivedFrom__FP5BASIC3CID(o, 0x22))
            k = 0;
        else if (FIsBasicDerivedFrom__FP5BASIC3CID(o, 0x26))
            k = e->x0->x10c;
        else
            k = FIsBasicDerivedFrom__FP5BASIC3CID(o, 0x23) ? 7 : -1;
        if (k != -1) {
            SetJtGrabState(p, k, o, e->x4);
            return 1;
        }
    }
    return 0;
}
