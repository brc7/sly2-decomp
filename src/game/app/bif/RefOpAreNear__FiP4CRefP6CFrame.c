typedef struct CRef { int m_tagk; union { int n; float g; } u; } CRef;
int FFloatsNear__Ffff(float a, float b, float c);
/** @sly2-annot addr=0x120A58
 * Purpose: Splice builtin "near?": FFloatsNear(a,b,eps)
 * Info: proposed name RefOpAreNear (T1); TU bif
 * Confidence: high [T1 0.93] - evidence: static analysis; sources: r1_116918+verdicts_B+verify_side
 * See: docs/engine/splice-scripting.md#7-key-functions,
 *   docs/reference/functions/splice-scripting.md#bif-0x11f440-0x1258f0
 * @end-sly2-annot */
CRef *RefOpAreNear__FiP4CRefP6CFrame(CRef *ret, void *pbasic, CRef *aref)
{
    float ag[3];
    int i;
    int f;
    for (i = 0; i < 3; i++) {
        CRef *p = &aref[i];
        if (p->m_tagk == 0x15) ag[i] = p->u.n; else ag[i] = p->u.g;
    }
    f = FFloatsNear__Ffff(ag[0], ag[1], ag[2]) != 0;
    ret->m_tagk = 0x17;
    ret->u.n = f;
    return ret;
}
