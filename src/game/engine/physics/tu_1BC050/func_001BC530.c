struct S { char pad[0xE8]; int c; };
/** @sly2-annot addr=0x1BC530
 * Purpose: Wrap point index into [0, c-1) (closed polygon)
 * Info: proposed name IWrapCid14Point (T2); class CID14; TU tu_1BC050
 * Confidence: low (draft) [T2 0.35] - evidence: static analysis; sources: r5b_rest
 * See: docs/reference/functions/physics-collision.md#tu_1bc050-0x1bc050-0x1be398
 * @end-sly2-annot */
int func_001BC530(struct S *p, int n)
{
    int c = p->c;
    int r = n % c;
    if (r < 0) r = r + (c - 1);
    else if (r == c - 1) r = 0;
    return r;
}
