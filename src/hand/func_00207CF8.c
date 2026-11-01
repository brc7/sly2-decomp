long func_00207B10(int a);
void *func_00209C18(void *p, long id);
void func_00207C48(void *p, int a, int b, int c, int d, int e);
void func_00209130(void *pe, void *p);
/** @sly2-annot addr=0x207CF8
 * Purpose: 0x207C48 then resolve
 * Info: proposed name SetStepguardAnimBindingK3Resolve (T2); TU stepguard
 * Confidence: low (draft) [T2 0.35] - evidence: static analysis; sources: r7b_guard
 * See: docs/reference/functions/guards-ai.md#stepguard-0x205e70-0x20b840
 * @end-sly2-annot */
void func_00207CF8(void *p, int a, int b, int c, int d, int e)
{
    void *pe = func_00209C18(p, func_00207B10(a));
    func_00207C48(p, a, b, c, d, e);
    func_00209130(pe, p);
}
