struct B { char pad[0x18]; int n; };
struct N { char pad[0x1C]; struct N *next; int pad2; struct B *pb; int n; };
extern struct N *D_003E126C;
/** @sly2-annot addr=0x1F67E0
 * Purpose: Same search over D_003E126C (SMA live list per sm.md)
 * Info: proposed name PsmaFindLive (T2); TU so
 * Confidence: low (draft) [T2 0.35] - evidence: static analysis; sources: r6_1D5150
 * See: docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8
 * @end-sly2-annot */
struct N *func_001F67E0(int n, int m)
{
    struct N *p;
    int unused[4];
    for (p = D_003E126C; p; p = p->next) {
        if (p->n == n) {
            if (m == -1 || p->pb->n == m)
                return p;
        }
    }
    return 0;
}
