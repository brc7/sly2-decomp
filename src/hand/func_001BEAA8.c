struct S { char pad[0xE0]; int c; void *a[1]; };
/** @sly2-annot addr=0x1BEAA8
 * Purpose: Append object to list
 * Info: proposed name AddCid45Object (T2); class CID45; TU tu_1BE398
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r5b_rest
 * See: docs/reference/functions/rendering.md#tu_1be398-0x1be398-0x1bf110
 * @end-sly2-annot */
void func_001BEAA8(struct S *p, void *x)
{
    if (x) p->a[p->c++] = x;
}
