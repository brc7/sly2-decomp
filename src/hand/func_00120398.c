typedef struct CRef { int m_tagk; union { int n; float g; } u; } CRef;
/** @sly2-annot addr=0x120398
 * Purpose: Numeric compare of aref[0],aref[1] (int->float promote) by CMPK 0..3 (<,<=,>,>=) -> Bool
 * Info: proposed name RefCmp (T1); TU bif
 * Confidence: high [T1 0.92] - evidence: static analysis; sources: r1_116918+verdicts_A1
 * See: docs/engine/splice-scripting.md#7-key-functions,
 *   docs/reference/functions/splice-scripting.md#bif-0x11f440-0x1258f0
 * @end-sly2-annot */
CRef *func_00120398(CRef *ret, CRef *aref, int op)
{
    float ag[2];
    int i;
    int f;
    for (i = 0; i < 2; i++) {
        if (aref[i].m_tagk == 0x16)
            ag[i] = aref[i].u.g;
        else
            ag[i] = (float)aref[i].u.n;
    }
    switch (op) {
    case 0:
        f = ag[0] < ag[1];
        break;
    case 1:
        f = ag[0] <= ag[1];
        break;
    case 2:
        f = ag[0] > ag[1];
        break;
    case 3:
        f = ag[0] >= ag[1];
        break;
    }
    ret->u.n = f;
    ret->m_tagk = 0x17;
    return ret;
}
