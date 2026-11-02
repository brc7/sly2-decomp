struct I { int a, b; };
struct E { int id; int n; int c; struct I *a; };
struct S;
extern char D_002A76C0[];
struct E *func_001BBE68(struct S *p, void *k);
/** @sly2-annot addr=0x1BBDB0
 * Purpose: Append an action of kind x to the current stage of a CID98 object (max 64 per stage) and return it
 * Info: proposed name PactionAddCid98 (T2); class CID98; TU tu_1BAB50
 * Notes: names_v2 C proposed LoadHbskFromBrx__FP4HBSKP18CBinaryInputStream (0.197) | top50_round (0.45):
 *   Body; callers AddCid98Action*, AddCid98ShowGlobs/HideGlobs/ShowParts (Splice builders). Class placeholder
 *   -> T2 | T2: placeholder token Cid98
 * Confidence: low (draft) [T2 0.45] - evidence: semantic review; static naming pass; sources:
 *   r5b_rest+top50_round
 * See: docs/reference/functions/gameplay-objects.md#tu_1bab50-0x1bab50-0x1bc050
 * @end-sly2-annot */
struct I *func_001BBDB0(struct S *p, int x)
{
    struct E *pe = func_001BBE68(p, D_002A76C0);
    struct I *pi;
    if (!pe) return 0;
    if (pe->c >= 0x40) return 0;
    pi = &pe->a[pe->c++];
    pi->a = x;
    return pi;
}
