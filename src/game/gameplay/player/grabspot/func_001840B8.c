struct S { char pad[0x118]; int a; int b; };
extern char D_003DD490[];
void func_0020FEC0(void *g, void *pb, int n, void *pa, int x, int y, int z, int w);
/** @sly2-annot addr=0x1840B8
 * Purpose: FAddRsmg(g_sw, &p->b, 4, &p->a, 1, ...)
 * Info: proposed name AddCid35Rsmg (T2); class CID35; TU grabspot
 * Confidence: low (draft) [T2 0.35] - evidence: static analysis; sources: r4b_rest
 * See: docs/reference/functions/player.md#grabspot-0x182c70-0x1848a8
 * @end-sly2-annot */
void func_001840B8(struct S *p, int y, int z, int w)
{
    func_0020FEC0(D_003DD490, &p->b, 4, &p->a, 1, y, z, w);
}
