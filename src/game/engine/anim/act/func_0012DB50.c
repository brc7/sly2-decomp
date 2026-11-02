typedef struct T { int a; int b; int c; int d; int e; int f; } T;
T *func_0012E238(void *p);
/** @sly2-annot addr=0x12DB50
 * Purpose: Allocates/initialises an act record of a kind (0x12E238) for (alo, owner)
 * Info: proposed name PactNewKind (T2); TU act
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r1_116918
 * See: docs/reference/functions/animation.md#act-0x12b3b0-0x12e600
 * @end-sly2-annot */
T *func_0012DB50(void *p, int b, int c, int d, int f)
{
    T *t = func_0012E238(p);
    t->b = b;
    t->c = c;
    t->d = d;
    t->f = f;
    return t;
}
