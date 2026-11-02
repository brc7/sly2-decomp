struct G { char pad[0x3D60]; int n; };
extern struct G D_003DD490;
struct Q { char pad[0xC]; int n; };
struct A { char pad[0x134]; struct Q *pq; };
void func_001A4478(struct G *pg, int n, struct Q *pq);
/** @sly2-annot addr=0x1F3C10
 * Purpose: If bounds record (+0x134) stamp != g_sw+0x3D60, recompute via 0x1A4478 and store stamp
 * Info: proposed name RefreshSoBoundsLighting (T2); TU so
 * Notes: names_v2 B proposed SetSoNoXpsSelf__FP2SOi (0.856)
 * Confidence: low (draft) [T2 0.25] - evidence: static naming pass; sources: r6_1D5150
 * See: docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8,
 *   docs/reference/functions/player.md#stepplayer-0x13ff70-0x144638
 * @end-sly2-annot */
void func_001F3C10(struct A *p)
{
    struct Q *pq = p->pq;
    if (pq && pq->n != D_003DD490.n) {
        func_001A4478(&D_003DD490, 0, pq);
        p->pq->n = D_003DD490.n;
    }
}
