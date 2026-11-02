struct E { int a; int b; unsigned int p; int c; int d; };
extern struct E D_003DA798[];
/** @sly2-annot addr=0x1EC740
 * Purpose: Rebase pointers in D_003DA798 records [iFirst,iLast) that fall in [pOld,pOld+cb) to pNew
 * Info: proposed name RelocateShdRefs (T2); TU shd
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r6b_rest
 * See: docs/reference/functions/rendering.md#shd-0x1ea568-0x1ed7a8
 * @end-sly2-annot */
void func_001EC740(int iFirst, int iLast, int cb, unsigned int pOld, unsigned int pNew)
{
    int i;
    unsigned int pEnd = pOld + cb;
    for (i = iFirst; i < iLast; i++) {
        struct E *pe = &D_003DA798[i];
        if (pe->p >= pOld && pe->p < pEnd) pe->p = pNew + (pe->p - pOld);
    }
}
