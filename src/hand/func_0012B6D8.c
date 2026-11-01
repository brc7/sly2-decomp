typedef struct KEY { int pad[3]; float t; int pad2[8]; } KEY;
typedef struct T { int pad; int c; KEY *a; } T;
/** @sly2-annot addr=0x12B6D8
 * Purpose: Finds the key segment containing time t
 * Info: proposed name AcVecA_IkeyFind (T2); class AcVecA; TU act
 * Confidence: low (draft) [T2 0.35] - evidence: static analysis; sources: r1_116918
 * See: docs/reference/functions/animation.md#act-0x12b3b0-0x12e600
 * @end-sly2-annot */
int func_0012B6D8(T *t, float g)
{
    int i;
    for (i = 1; i < t->c - 1; i++) {
        if (g < t->a[i].t)
            break;
    }
    return i;
}
