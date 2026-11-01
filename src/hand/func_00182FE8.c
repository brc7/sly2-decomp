struct S { char pad[0xE4]; int x; };
float func_001E6620(struct S *p);
float func_001E6648(struct S *p);
/** @sly2-annot addr=0x182FE8
 * Purpose: If open shape: out [SMinShape, SMaxShape], return 1
 * Info: proposed name CID12_FGetGrabRange (T2); class CID12; TU grabspot
 * Notes: superseded: CID12_FGetWrapRange (r4b_rest) | GRABSPOT secondary slot 1 named FGetGrabRange
 *   everywhere (verifier r4b: all overrides return a [min,max] range)
 * Confidence: low (draft) [T2 0.50] - evidence: static analysis; sources: r4b_rest+CONFLICTS#15
 * See: docs/reference/functions/player.md#grabspot-0x182c70-0x1848a8
 * @end-sly2-annot */
int func_00182FE8(struct S *p, float *pg)
{
    if (p->x) {
        pg[0] = func_001E6620(p);
        pg[1] = func_001E6648(p);
        return 1;
    }
    return 0;
}
