typedef int TI __attribute__((mode(TI)));
typedef struct CRef { int m_tagk; int m_tag; } CRef;
void Decref__4CRef(CRef *pref);
CRef *__4CRefRC4CRef(CRef *pref, CRef *prefSrc);
void func_00129248(CRef *pref, TI vec);
TI func_0011BFF0(int n);
/** @sly2-annot addr=0x1230C8
 * Purpose: Splice builtin "matrix-decompose-to-euler": 0x11BFF0 euler
 * Info: proposed name RefOpMatrixDecomposeToEuler (T1); TU bif
 * Confidence: high [T1 0.93] - evidence: static analysis; sources: r1_116918
 * See: docs/reference/functions/splice-scripting.md#bif-0x11f440-0x1258f0
 * @end-sly2-annot */
CRef *func_001230C8(CRef *ret, void *pbasic, CRef *aref)
{
    CRef ref;
    func_00129248(&ref, func_0011BFF0(aref->m_tag));
    __4CRefRC4CRef(ret, &ref);
    Decref__4CRef(&ref);
    return ret;
}
