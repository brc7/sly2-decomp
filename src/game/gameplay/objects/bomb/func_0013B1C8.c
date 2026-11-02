struct N { char pad[0x10]; struct N *pnext; };
extern int D_002BC9D0[4];
int FIsBasicDerivedFrom__FP5BASIC3CID(struct N *, int);
/** @sly2-annot addr=0x13B1C8
 * Purpose: 0 if any object in the chain derives from one of 4 classes (D_002BC9D0), else 1
 * Info: proposed name FBombChainHasNoExcludedClass (T2); TU bomb
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r2b_rest
 * See: docs/reference/functions/gameplay-objects.md#bomb-0x13a9e8-0x13bdf0
 * @end-sly2-annot */
int func_0013B1C8(void *unused, struct N *pn) {
    unsigned int i;
    for (; pn; pn = pn->pnext) {
        for (i = 0; i < 4; i++) {
            if (FIsBasicDerivedFrom__FP5BASIC3CID(pn, D_002BC9D0[i])) return 0;
        }
    }
    return 1;
}
