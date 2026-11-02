typedef struct CRef { int m_tagk; int m_tag; } CRef;
typedef float (*PFNG)(float);
extern PFNG D_002BBB70[];
void SetF32__4CReff(CRef *pref, float g);
CRef *func_001291A8(CRef *pref, CRef *prefSrc);
void func_00129EC8(CRef *pref);
/** @sly2-annot addr=0x1224A8
 * Purpose: Unary float op dispatcher (sqrt, sin, cos, tan, asin, acos, atan, rad-normalize by UFOK 0..7)
 * Info: proposed name RefUfo (T1); TU bif
 * Confidence: high [T1 0.90] - evidence: static analysis; sources: r1_116918
 * See: docs/engine/splice-scripting.md#4-builtin-table-d_002bbb90,
 *   docs/reference/functions/splice-scripting.md#bif-0x11f440-0x1258f0
 * @end-sly2-annot */
CRef *RefUfo__FP4CRef4UFOK(CRef *ret, CRef *parg, int i)
{
    CRef ref;
    float g;
    ref.m_tagk = -1;
    if (parg->m_tagk == 0x16) {
        g = *(float *)&parg->m_tag;
    } else {
        g = (float)parg->m_tag;
    }
    SetF32__4CReff(&ref, D_002BBB70[i](g));
    func_001291A8(ret, &ref);
    func_00129EC8(&ref);
    return ret;
}
