typedef struct QW { int a[4]; } QW;
struct O { char pad[0xFC]; int c; QW a[1]; };
struct S { char pad[0x3A4]; int n; char p2[0xF64 - 0x3A8]; struct O *o; };
QW *PdteFindDetectorTarget(struct O *o);
void CopyAb__FPvT0Ui(void *dst, void *src, int cb);
/** @sly2-annot addr=0x208860
 * Purpose: Remove a non-generated detector entry (compact)
 * Info: proposed name RemoveStepguardDetectTarget (T2); TU stepguard
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r7b_guard
 * See: docs/reference/functions/guards-ai.md#stepguard-0x205e70-0x20b840
 * @end-sly2-annot */
void func_00208860(struct S *p)
{
    QW *pq = PdteFindDetectorTarget(p->o);
    struct O *o = p->o;
    int c = o->c;
    QW *pqEnd = &o->a[c - p->n];
    if (pq && pq < pqEnd) {
        int i;
        o->c = c - 1;
        i = pq - o->a;
        CopyAb__FPvT0Ui(pq, pq + 1, (p->o->c - i) * 16);
    }
}
