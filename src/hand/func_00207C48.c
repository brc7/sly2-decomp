struct X { int a; char p0[0x2C]; int b; char p1[0x2C]; int c; char p2[0x2C]; int d; };
struct E { int k; struct X *px; int pad; int n; };
long func_00207B10(int a);
struct E *func_00209C18(void *p, long id);
struct X *func_0020CA00(int cb);
void func_0020C7D8(struct X *px);
/** @sly2-annot addr=0x207C48
 * Purpose: Splice: binding of kind 3 (new 0xD0 object 0x20CA00 with 4 params), n=8
 * Info: proposed name SetStepguardAnimBindingK3 (T2); TU stepguard
 * Confidence: low (draft) [T2 0.35] - evidence: static analysis; sources: r7b_guard
 * See: docs/reference/functions/guards-ai.md#stepguard-0x205e70-0x20b840
 * @end-sly2-annot */
void func_00207C48(void *p, int id, int a, int b, int c, int d)
{
    struct E *pe = func_00209C18(p, func_00207B10(id));
    pe->k = 3;
    pe->px = func_0020CA00(0xD0);
    func_0020C7D8(pe->px);
    pe->px->a = a;
    pe->px->b = b;
    pe->px->c = c;
    pe->px->d = d;
    pe->n = 8;
}
