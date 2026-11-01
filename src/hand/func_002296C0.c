struct N { int a[3]; struct N *next; };
void func_0011CD98(void *pool, struct N *n);
/** @sly2-annot addr=0x2296C0
 * Purpose: frees a linked list of XA nodes into g_sw slotheapXa
 * Info: proposed name FreeXaList (T2); TU water
 * Confidence: low (draft) [T2 0.45] - evidence: static analysis; sources: r8_21BB88
 * See: docs/reference/functions/physics-collision.md#water-0x228b48-0x229ef0
 * @end-sly2-annot */
void func_002296C0(char *p, struct N *n)
{
    while (n) {
        struct N *next = n->next;
        func_0011CD98(p + 0x40E8, n);
        n = next;
    }
}
