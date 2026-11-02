typedef int qword __attribute__((mode(TI)));
struct S { char pad[0x4020]; int a; int b; };
void func_00156BE0(int n, qword q, int c, int d);
/** @sly2-annot addr=0x156C80
 * Purpose: ReplaceHeldNoise using SW template 0 or 1
 * Info: proposed name ReplaceHeldSwNoise (T2); TU noise
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r3b_rest
 * See: docs/reference/functions/guards-ai.md#noise-0x1564e8-0x157200
 * @end-sly2-annot */
void func_00156C80(struct S *p, int k, qword q, int c, int d)
{
    int n;
    switch (k) {
    case 0: n = p->a; break;
    case 1: n = p->b; break;
    default: n = 0; break;
    }
    func_00156BE0(n, q, c, d);
}
