typedef struct T { int pad; int c; float *a; } T;
/** @sly2-annot addr=0x12C708
 * Purpose: Key segment search
 * Info: proposed name AcFloatB_IkeyFind (T2); class AcFloatB; TU act
 * Confidence: low (draft) [T2 0.35] - evidence: static analysis; sources: r1_116918
 * See: docs/reference/functions/animation.md#act-0x12b3b0-0x12e600
 * @end-sly2-annot */
int func_0012C708(T *t, float g)
{
    int i;
    for (i = 1; i < t->c - 1; i++) {
        if (g < t->a[i])
            break;
    }
    return i;
}
