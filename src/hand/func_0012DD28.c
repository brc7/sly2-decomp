typedef struct T { int a[7]; void *q; } T;
typedef struct Q { int pad[2]; void *b; } Q;
T *func_0012DB50(int k, void *a, void *b, void *c, int d);
void AddXfAct(void *a, void *b, T *p);
/** @sly2-annot addr=0x12DD28
 * Purpose: Create act variant and attach
 * Info: proposed name PactNewC (T2); TU act
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r1_116918+verdicts_tu_r1r2
 * See: docs/engine/animation.md#7-key-functions, docs/reference/functions/animation.md#act-0x12b3b0-0x12e600
 * @end-sly2-annot */
T *func_0012DD28(void *a, void *b, void *c, Q *q)
{
    T *p = func_0012DB50(7, a, b, c, b == q->b ? 0xE : 0xD);
    p->q = q;
    AddXfAct(a, b, p);
    return p;
}
