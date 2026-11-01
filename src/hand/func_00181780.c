struct VTE { short d; short pad; void (*pfn)(void *, int, int, int, int, int); };
struct S { char pad[0xC]; struct VTE *vt; char p2[0x1F0]; int a; int b; };
extern char D_003DD490[];
void TriggerRsmg(void *g, int a, int *pb, struct S *p, int n);
/** @sly2-annot addr=0x181780
 * Purpose: TriggerRsmg on its table
 * Info: proposed name TriggerHacklockRsmg (T2); class HACKLOCK; TU tu_17F090
 * Notes: names_v2 C proposed UpdateSggCallback__FP3SGG5MSGIDPv (0.438) | Cid4B->Cid75 (CID 0x4B=75,
 *   hex-literal) | class identity from whole_game WE names/object strings: TriggerCid75Rsmg ->
 *   TriggerHacklockRsmg
 * Confidence: low (draft) [T2 0.30] - evidence: static naming pass; sources: r4b_rest+whole_game(class
 *   identity)
 * See: docs/reference/functions/gameplay-objects.md#tu_17f090-0x17f090-0x182c70
 * @end-sly2-annot */
void func_00181780(struct S *p)
{
    struct VTE *e;
    TriggerRsmg(D_003DD490, p->a, &p->b, p, 1);
    e = &p->vt[3];
    e->pfn((char *)p + e->d, 5, 0, 0, 0, 0);
}
