extern int D_002C3D10[];
extern char D_003DD490[];
int CploFindSwObjectsByClass(void *p, int a, int b, int c, int d, int e);
void func_00208920(void *p, int n);
/** @sly2-annot addr=0x206DB0
 * Purpose: Add those 5 guard cids (when present in the world) to the ignore-class list +0x3A8
 * Info: proposed name AddStepguardRageIgnoreClasses (T2); TU stepguard
 * Confidence: low (draft) [T2 0.35] - evidence: static analysis; sources: r7b_guard
 * See: docs/reference/functions/guards-ai.md#stepguard-0x205e70-0x20b840
 * @end-sly2-annot */
void func_00206DB0(void *p)
{
    unsigned int i;
    for (i = 0; i < 5; i++) {
        int n = D_002C3D10[i];
        if (CploFindSwObjectsByClass(D_003DD490, 0x605, n, 0, 0, 0))
            func_00208920(p, n);
    }
}
