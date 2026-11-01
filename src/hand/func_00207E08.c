long func_00207B10(int a);
void *func_00209C18(void *p, long id);
void func_00207D98(void *p, int a, int b, float c, float d, float e, float f);
void ResolveAnimBinding(void *pe, void *p);
/** @sly2-annot addr=0x207E08
 * Purpose: 0x207D98 then resolve
 * Info: proposed name SetStepguardAnimBindingK2Resolve (T2); TU stepguard
 * Confidence: low (draft) [T2 0.35] - evidence: static analysis; sources: r7b_guard
 * See: docs/reference/functions/guards-ai.md#stepguard-0x205e70-0x20b840
 * @end-sly2-annot */
void func_00207E08(void *p, int a, int b, float c, float d, float e, float f)
{
    void *pe = func_00209C18(p, func_00207B10(a));
    func_00207D98(p, a, b, c, d, e, f);
    ResolveAnimBinding(pe, p);
}
