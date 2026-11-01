struct S { int c; int a[0x100]; };
/** @sly2-annot addr=0x1D57E0
 * Purpose: Pop int (0 if empty)
 * Info: proposed name PopIntStack (T2); TU tu_1D57A8
 * Confidence: low (draft) [T2 0.45] - evidence: static analysis; sources: r6_1D5150
 * See: docs/reference/functions/rendering.md#tu_1d57a8-0x1d57a8-0x1d6848
 * @end-sly2-annot */
int func_001D57E0(struct S *p) {
    if (p->c == 0) return 0;
    return p->a[--p->c];
}
