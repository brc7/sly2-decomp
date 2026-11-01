typedef struct VT { short d; short pad; void (*fn)(void *); } VT;
typedef struct O { int pad[6]; VT *vt; } O;
typedef struct T { int pad[7]; O *q; } T;
/** @sly2-annot addr=0x12D890
 * Purpose: vtable 0x2CA758 slot 2
 * Info: proposed name ActUnk_2CA758_2 (T2); TU act
 * Confidence: low (draft) [T2 0.20] - evidence: static analysis; sources: r1_116918
 * See: docs/reference/functions/animation.md#act-0x12b3b0-0x12e600
 * @end-sly2-annot */
void func_0012D890(T *t)
{
    O *p = t->q;
    if (p) {
        p->vt[2].fn((char *)p + p->vt[2].d);
        t->q = 0;
    }
}
