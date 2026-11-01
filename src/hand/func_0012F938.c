typedef struct E { int a; int k; int n; int b; } E;
typedef struct T { char pad[0xA0]; int c; E *a; } T;
/** @sly2-annot addr=0x12F938
 * Purpose: Finds the label/event entry with a given id in the ASEG table
 * Info: proposed name PeaFindAsegLabel (T1); TU aseg
 * Confidence: mid (draft) [T1 0.65] - evidence: static analysis; sources: r1_116918
 * See: docs/reference/functions/animation.md#aseg-0x12f248-0x1308a8
 * @end-sly2-annot */
E *func_0012F938(T *t, int n)
{
    int i;
    E *e;
    for (i = 0; i < t->c; i++) {
        e = &t->a[i];
        if ((e->k == 0xB) & (e->n == n))
            return e;
    }
    return 0;
}
