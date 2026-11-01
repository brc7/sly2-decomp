struct S { char pad[0x50]; int n50; int n54; int n58; char pad2[0x80-0x5C]; int n80; };
/** @sly2-annot addr=0x1E01C8
 * Purpose: If 0<n<0x200 and at most one command so far: crelaycmdMax=n and mark private limit (+0x80)
 * Info: proposed name SetRelayCmdMax (T2); TU relay
 * Confidence: low (draft) [T2 0.45] - evidence: static analysis; sources: r6b_rest
 * See: docs/reference/functions/cutscenes.md#relay-0x1dfad8-0x1e5d80
 * @end-sly2-annot */
void func_001E01C8(struct S *p, int n)
{
    if (n > 0) {
        if (n < 0x200 && p->n50 < 2) {
            p->n58 = n;
            p->n80 = 1;
        }
    }
}
