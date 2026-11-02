struct S { char pad[0xE0]; int c; char *a[1]; };
void func_00169198(void *p, struct S *ps);
/** @sly2-annot addr=0x1BEE60
 * Purpose: CID 45 vt19: if it owns a slot, draw each held object's globset (0x169198)
 * Info: proposed name RenderCid45Self (T2); class CID45 slot19; TU tu_1BE398
 * Confidence: low (draft) [T2 0.45] - evidence: static analysis; sources: r5b_rest
 * See: docs/reference/functions/rendering.md#tu_1be398-0x1be398-0x1bf110
 * @end-sly2-annot */
void func_001BEE60(struct S *p)
{
    int i;
    if (*(int *)((char *)p + 0x128)) {
        for (i = 0; i < p->c; i++) func_00169198(p->a[i] + 0x80, p);
    }
}
