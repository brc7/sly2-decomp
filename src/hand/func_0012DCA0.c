typedef struct T { int a[7]; float x; float y; } T;
T *func_0012DB50(int k, void *a, void *b, void *c, void *d);
void func_00230018(void *a, void *b, T *p);
/** @sly2-annot addr=0x12DCA0
 * Purpose: Create act variant and attach
 * Info: proposed name PactNewB (T2); TU act
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r1_116918+verdicts_tu_r1r2
 * See: docs/engine/animation.md#7-key-functions, docs/reference/functions/animation.md#act-0x12b3b0-0x12e600
 * @end-sly2-annot */
T *func_0012DCA0(void *a, void *b, void *c, void *d, float x, float y)
{
    T *p = func_0012DB50(4, a, b, c, d);
    p->x = x;
    p->y = y;
    func_00230018(a, b, p);
    return p;
}
