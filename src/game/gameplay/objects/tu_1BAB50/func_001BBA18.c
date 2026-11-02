struct S { char pad[0x15C]; int n; };
struct R { int a; int b; };
extern char D_002A76B8[];
struct R *func_001BBE68(struct S *p, void *k);
/** @sly2-annot addr=0x1BBA18
 * Purpose: Set a field of the current stage record (D_002A76B8 key)
 * Info: proposed name SetCid98StageParam (T2); class CID98; TU tu_1BAB50
 * Confidence: low (draft) [T2 0.25] - evidence: static analysis; sources: r5b_rest
 * See: docs/reference/functions/gameplay-objects.md#tu_1bab50-0x1bab50-0x1bc050
 * @end-sly2-annot */
void func_001BBA18(struct S *p, int x)
{
    if (p->n != -1) {
        struct R *r = func_001BBE68(p, D_002A76B8);
        if (r && x) r->b = x;
    }
}
