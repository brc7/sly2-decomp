struct S { int n; };
extern struct S **D_003DD018;
/** @sly2-annot addr=0x1ED5D0
 * Purpose: Look up a clut by id
 * Info: proposed name PclutFindHash (T2); TU shd
 * Confidence: low (draft) [T2 0.45] - evidence: static analysis; sources: r6b_rest
 * See: docs/formats/textures.md#2-clut-record-0x10-byte-header-cstatic,
 *   docs/reference/functions/rendering.md#shd-0x1ea568-0x1ed7a8
 * @end-sly2-annot */
struct S *func_001ED5D0(int n)
{
    int i;
    struct S *p;
    if (!D_003DD018) return 0;
    i = n % 0x3FFD;
loop:
    p = D_003DD018[i];
    if (!p || p->n == n) return p;
    i = (i + 1) % 0x3FFD;
    goto loop;
}
