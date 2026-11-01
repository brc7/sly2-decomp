struct S { char pad[0x3C0]; int c; int a[1]; };
int FIsBasicDerivedFrom__FP5BASIC3CID(void *o, int n);
/** @sly2-annot addr=0x208B38
 * Purpose: plo derives from a cid in +0x3C0
 * Info: proposed name FStepguardForceClass (T2); TU stepguard
 * Confidence: low (draft) [T2 0.35] - evidence: static analysis; sources: r7b_guard
 * See: docs/reference/functions/guards-ai.md#stepguard-0x205e70-0x20b840
 * @end-sly2-annot */
int func_00208B38(struct S *p, void *o)
{
    int i;
    for (i = 0; i < p->c; i++) {
        if (FIsBasicDerivedFrom__FP5BASIC3CID(o, p->a[i])) return 1;
    }
    return 0;
}
