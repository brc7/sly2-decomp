typedef struct T { int a; int b; int c; int d; int e; int f; int g; char ab[1]; } T;
typedef struct G { int pad[3]; unsigned int cb; } G;
extern G D_003E1230;
void CopyAb__FPvT0Ui(void *dst, void *src, unsigned int cb);
/** @sly2-annot addr=0x12CF88
 * Purpose: Copies header fields and CopyAb the class body (size from D_003E1230.cb)
 * Info: proposed name CloneAct (T1); class ACT; TU act
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: r1_116918
 * See: docs/reference/functions/animation.md#act-0x12b3b0-0x12e600,
 *   docs/reference/functions/animation.md#tail-0x21c820-0x21d278
 * @end-sly2-annot */
void CloneAct(T *t, T *src, int b, int c)
{
    t->a = src->a;
    t->d = src->d;
    t->f = src->f;
    t->b = b;
    t->c = c;
    CopyAb__FPvT0Ui(t->ab, src->ab, D_003E1230.cb - 0x1C);
}
