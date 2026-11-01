typedef struct M4 { int q[16]; } __attribute__((aligned(16))) M4;
typedef struct T { int a[7]; int n; M4 mat; } T;
T *func_0012DB50(int k, void *a, void *b, void *c, void *d);
void AddXfAct(void *a, void *b, T *p);
/** @sly2-annot addr=0x12DDB0
 * Purpose: Create act variant and attach
 * Info: proposed name PactNewD (T2); TU act
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r1_116918+verdicts_tu_r1r2
 * See: docs/engine/animation.md#7-key-functions, docs/reference/functions/animation.md#act-0x12b3b0-0x12e600
 * @end-sly2-annot */
T *func_0012DDB0(void *a, void *b, void *c, void *d, int n, M4 *pmat)
{
    T *p = func_0012DB50(6, a, b, c, d);
    p->n = n;
    p->mat = *pmat;
    AddXfAct(a, b, p);
    return p;
}
