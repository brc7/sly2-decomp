typedef struct X { int n0; char pad[0x30]; int n34; int n38; } X;

/** @sly2-annot addr=0x118500
 * Purpose: Stores buffer pointer/size (+0x34/+0x38) and +0 state
 * Info: proposed name CMemInputStream_SetBuffer (T2); class CMemInputStream; TU bis
 * Confidence: low (draft) [T2 0.45] - evidence: static analysis; sources: r1_116918
 * See: docs/reference/functions/core-library.md#bis-0x1177a8-0x118668
 * @end-sly2-annot */
void func_00118500(X *px, int n34, int n38, int n0)
{
    px->n34 = n34;
    px->n38 = n38;
    px->n0 = n0;
}
