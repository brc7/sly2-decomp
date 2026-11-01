struct S { char pad[0x394]; int c; int a[1]; };
int FIsBasicDerivedFrom__FP5BASIC3CID(void *o, int n);
/** @sly2-annot addr=0x2087E8
 * Purpose: plo derives from a cid in +0x394
 * Info: proposed name FStepguardPreferClass (T2); TU stepguard
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r7b_guard
 * See: docs/reference/functions/guards-ai.md#stepguard-0x205e70-0x20b840
 * @end-sly2-annot */
int func_002087E8(struct S *p, void *o)
{
    int i;
    for (i = 0; i < p->c; i++) {
        if (FIsBasicDerivedFrom__FP5BASIC3CID(o, p->a[i])) return 1;
    }
    return 0;
}
