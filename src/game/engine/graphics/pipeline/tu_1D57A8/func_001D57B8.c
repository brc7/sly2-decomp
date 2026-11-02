struct S { unsigned int c; int a[0x100]; };
/** @sly2-annot addr=0x1D57B8
 * Purpose: Push int on 256-entry stack if not full
 * Info: proposed name PushIntStack (T2); TU tu_1D57A8
 * Confidence: low (draft) [T2 0.45] - evidence: static analysis; sources: r6_1D5150
 * See: docs/reference/functions/rendering.md#tu_1d57a8-0x1d57a8-0x1d6848
 * @end-sly2-annot */
void func_001D57B8(struct S *p, int n) {
    if (p->c < 0x100) {
        p->a[p->c++] = n;
    }
}
