struct B { char pad[0x18]; int n; };
struct N { char pad[0x14]; struct B *pb; int n; char pad2[8]; struct N *next; };
extern struct N *D_003E1218;
/** @sly2-annot addr=0x1F6788
 * Purpose: Search list D_003E1218 for node with n==oid and (m==-1 or owner->n==m)
 * Info: proposed name PsmaFindByOidAndSm (T2); TU so
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r6_1D5150
 * See: docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8
 * @end-sly2-annot */
struct N *func_001F6788(int n, int m)
{
    struct N *p;
    int unused[4];
    for (p = D_003E1218; p; p = p->next) {
        if (p->n == n) {
            if (m == -1 || p->pb->n == m)
                return p;
        }
    }
    return 0;
}
