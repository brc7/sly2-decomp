struct S { char pad[0x200]; int a; int b; };
extern char D_003DD490[];
void FAddRsmg__FP2SWP4RSMGiPii3OIDN25(void *g, void *pb, int n, void *pa, int x, int y, int z, int w);
/** @sly2-annot addr=0x1816E8
 * Purpose: FAddRsmg on its RSMG table
 * Info: proposed name AddHacklockRsmg (T2); class HACKLOCK; TU tu_17F090
 * Notes: Cid4B->Cid75 (CID 0x4B=75, hex-literal) | class identity from whole_game WE names/object strings:
 *   AddCid75Rsmg -> AddHacklockRsmg
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r4b_rest+whole_game(class identity)
 * See: docs/reference/functions/gameplay-objects.md#tu_17f090-0x17f090-0x182c70
 * @end-sly2-annot */
void func_001816E8(struct S *p, int x, int y, int z, int w)
{
    FAddRsmg__FP2SWP4RSMGiPii3OIDN25(D_003DD490, &p->b, 4, &p->a, x, y, z, w);
}
