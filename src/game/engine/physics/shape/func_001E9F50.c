typedef int qword __attribute__((mode(TI)));
void *PpathzoneFromPos(qword q, int n);
int func_001C1700(void *p, int n, qword q, qword q2, int c, void **ap);
int FPosInCnvo(void *p, qword q);
/** @sly2-annot addr=0x1E9F50
 * Purpose: Find an object at pos (0x1C0D70/0x1C1700) passing 0x1BC9F0
 * Info: proposed name PvFindValidAtPos (T2); TU shape
 * Confidence: low (draft) [T2 0.25] - evidence: static analysis; sources: r6b_rest
 * See: docs/reference/functions/physics-collision.md#shape-0x1e5d80-0x1ea568
 * @end-sly2-annot */
void *func_001E9F50(qword q)
{
    void *ap[0x40];
    void *p = PpathzoneFromPos(q, 0);
    int c, i;
    if (!p) return 0;
    c = func_001C1700(p, 0, q, q, 0x40, ap);
    for (i = 0; i < c; i++) {
        void *pT = ap[i];
        if (FPosInCnvo(pT, q)) return pT;
    }
    return 0;
}
