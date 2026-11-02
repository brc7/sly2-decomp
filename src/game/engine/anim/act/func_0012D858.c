typedef struct VT { short d; short pad; void (*fn)(void *); } VT;
typedef struct O { int pad[6]; VT *vt; } O;
typedef struct T { int pad[4]; O *p; int pad2[2]; O *q; } T;
/** @sly2-annot addr=0x12D858
 * Purpose: vtable 0x2CA758 slot 1
 * Info: proposed name ActUnk_2CA758_1 (T2); TU act
 * Confidence: low (draft) [T2 0.20] - evidence: static analysis; sources: r1_116918
 * See: docs/reference/functions/animation.md#act-0x12b3b0-0x12e600
 * @end-sly2-annot */
void func_0012D858(T *t)
{
    O *p = t->p;
    t->q = p;
    if (p)
        p->vt[1].fn((char *)p + p->vt[1].d);
}
