struct S { char pad[0x15C]; int a; int b; };
extern char D_002DEB30[];
void func_00161F10(void *g, void *pb, int n, void *pa, int x, int y);
/** @sly2-annot addr=0x1840F8
 * Purpose: FAddPair(D_002DEB30, ...)
 * Info: proposed name AddCid35Pair (T2); class CID35; TU grabspot
 * Confidence: low (draft) [T2 0.35] - evidence: static analysis; sources: r4b_rest
 * See: docs/reference/functions/player.md#grabspot-0x182c70-0x1848a8
 * @end-sly2-annot */
void func_001840F8(struct S *p, int x, int y)
{
    func_00161F10(D_002DEB30, &p->b, 4, &p->a, x, y);
}
