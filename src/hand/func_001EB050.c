struct S { int a, b, c, d; unsigned int n10; int n14; unsigned int n18; };
struct T { int a, b, c, d; unsigned int n10; int n14; };
/** @sly2-annot addr=0x1EB050
 * Purpose: Copy a BMP header and compute qword count
 * Info: proposed name CopyBmpHeader (T2); TU shd
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r6b_rest
 * See: docs/reference/functions/rendering.md#shd-0x1ea568-0x1ed7a8
 * @end-sly2-annot */
struct S *func_001EB050(struct S *p, struct T *q)
{
    *(struct T *)p = *q;
    p->n18 = (p->n10 + 15) >> 4;
    return p;
}
