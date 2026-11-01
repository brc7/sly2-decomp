struct S { char pad[0x200]; int a; int b; };
extern char D_003DD490[];
void func_0017F6B0(struct S *p);
void func_001815E8(struct S *p, int n);
void func_0020FF70(void *g, int a, int *pb, struct S *p, int n);
/** @sly2-annot addr=0x181598
 * Purpose: Reset health + state
 * Info: proposed name ResetHacklock (T2); class HACKLOCK slot83; TU tu_17F090
 * Notes: names_v2 C proposed EnsureSggCallback__FP3SGG (0.438) | Cid4B->Cid75 (CID 0x4B=75, hex-literal) |
 *   class identity from whole_game WE names/object strings: ResetCid75 -> ResetHacklock
 * Confidence: low (draft) [T2 0.30] - evidence: static naming pass; sources: r4b_rest+whole_game(class
 *   identity)
 * See: docs/reference/functions/gameplay-objects.md#tu_17f090-0x17f090-0x182c70
 * @end-sly2-annot */
void func_00181598(struct S *p)
{
    func_0017F6B0(p);
    func_001815E8(p, 0);
    func_0020FF70(D_003DD490, p->a, &p->b, p, 0);
}
