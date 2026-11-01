void *func_0012DB50(int k, void *a, void *b, void *c, int d);
void func_00230018(void *a, void *b, void *p);
/** @sly2-annot addr=0x12DBB0
 * Purpose: Create act (variant A) and attach (0x230018)
 * Info: proposed name PactNewA (T2); TU act
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r1_116918
 * See: docs/reference/functions/animation.md#act-0x12b3b0-0x12e600
 * @end-sly2-annot */
void *func_0012DBB0(void *a, void *b, void *c)
{
    void *p = func_0012DB50(5, a, b, c, -1);
    func_00230018(a, b, p);
    return p;
}
