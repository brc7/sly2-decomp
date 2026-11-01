struct X { int a; };
struct S { int n; struct X *px; int b; int c; };
unsigned long func_00207B10(void *p);
struct S *func_00209C18(void *p, unsigned long m);
struct X *func_0020C010(int cb);
void func_0020BD78(struct X *px);
/** @sly2-annot addr=0x207D98
 * Purpose: Splice: binding of kind 2 (new 0x40 object 0x20C010 with one param)
 * Info: proposed name SetStepguardAnimBindingK2 (T2); TU stepguard
 * Confidence: low (draft) [T2 0.35] - evidence: static analysis; sources: r7b_guard
 * See: docs/reference/functions/guards-ai.md#stepguard-0x205e70-0x20b840
 * @end-sly2-annot */
void func_00207D98(void *p, void *q, int a)
{
    struct S *s = func_00209C18(p, func_00207B10(q));
    s->n = 2;
    s->px = func_0020C010(0x40);
    func_0020BD78(s->px);
    s->px->a = a;
    s->c = 0;
}
