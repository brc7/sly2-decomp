typedef struct VT { short d; short pad; void (*fn)(void *); } VT;
typedef struct O { int pad[6]; VT *vt; } O;
typedef struct T { int pad; void *a; void *b; int pad2; O *p; int pad3[2]; O *q; } T;
void func_0022C840(void *a, void *b);
/** @sly2-annot addr=0x12D8D8
 * Purpose: vtable 0x2CA758 slot 3
 * Info: proposed name ActUnk_2CA758_3 (T2); TU act
 * Confidence: low (draft) [T2 0.20] - evidence: static analysis; sources: r1_116918
 * See: docs/reference/functions/animation.md#act-0x12b3b0-0x12e600
 * @end-sly2-annot */
void func_0012D8D8(T *t)
{
    O *p;
    if (t->q != t->p) {
        func_0022C840(t->a, t->b);
        p = t->q;
        if (p)
            p->vt[2].fn((char *)p + p->vt[2].d);
        p = t->p;
        t->q = p;
        if (p)
            p->vt[1].fn((char *)p + p->vt[1].d);
    }
}
