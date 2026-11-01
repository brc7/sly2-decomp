typedef struct CRef { int m_tagk; int m_tag; } CRef;
void Decref__4CRef(CRef *pref);
CRef *__4CRefRC4CRef(CRef *pref, CRef *prefSrc);
void InitTag__4CRef(CRef *pref, int tagk);
typedef struct MATRIX4 { float m[4][4]; } __attribute__((aligned(16))) MATRIX4;
void func_0011C1D8(MATRIX4 *pmat, int n);
void func_001292B0(CRef *pref, MATRIX4 *pmat);
/** @sly2-annot addr=0x122DE0
 * Purpose: Splice builtin "matrix-transpose": 0x11C1D8
 * Info: proposed name RefOpMatrixTranspose (T1); TU bif
 * Confidence: high [T1 0.93] - evidence: static analysis; sources: r1_116918+verdicts_B
 * See: docs/engine/splice-scripting.md#7-key-functions,
 *   docs/reference/functions/splice-scripting.md#bif-0x11f440-0x1258f0
 * @end-sly2-annot */
CRef *func_00122DE0(CRef *ret, void *pbasic, CRef *aref)
{
    CRef ref;
    MATRIX4 mat;
    func_0011C1D8(&mat, aref->m_tag);
    func_001292B0(&ref, &mat);
    __4CRefRC4CRef(ret, &ref);
    Decref__4CRef(&ref);
    return ret;
}
