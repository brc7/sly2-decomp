struct S { int k; void *pv; };
void *func_0020C010(int cb);
void *func_0020CA00(int cb);
void CopyAb__FPvT0Ui(void *dst, void *src, int cb);
/** @sly2-annot addr=0x209590
 * Purpose: Deep-copy kind-2 (0x40) / kind-3 (0xD0) payloads
 * Info: proposed name CloneAnimBinding (T2); TU stepguard
 * Confidence: low (draft) [T2 0.45] - evidence: static analysis; sources: r7b_guard
 * See: docs/reference/functions/guards-ai.md#stepguard-0x205e70-0x20b840
 * @end-sly2-annot */
void func_00209590(struct S *p)
{
    void *pv;
    switch (p->k) {
    case 2:
        pv = func_0020C010(0x40);
        CopyAb__FPvT0Ui(pv, p->pv, 0x40);
        p->pv = pv;
        break;
    case 3:
        pv = func_0020CA00(0xD0);
        CopyAb__FPvT0Ui(pv, p->pv, 0xD0);
        p->pv = pv;
        break;
    }
}
