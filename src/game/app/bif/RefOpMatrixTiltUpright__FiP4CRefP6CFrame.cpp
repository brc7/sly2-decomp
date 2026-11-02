typedef int qword __attribute__((mode(TI)));
struct CRef {
    int m_tagk; union { int n; float g; void *pv; } u;
    CRef() { m_tagk = -1; }
    CRef(int tagk) { InitTag(tagk); }
    CRef(const CRef &r);
    void Decref();
    void InitTag(int tagk);
    ~CRef() { Decref(); }
    CRef &operator=(const CRef &r);
};
struct VECTOR { float x, y, z, w; } __attribute__((aligned(16)));
struct MATRIX3 { VECTOR mat[3]; };
struct MATRIX4 { VECTOR mat[3]; VECTOR pos; };
struct XF { char pad[0x30]; qword pos; };
extern "C" {
void TiltMatUpright(XF *pxf, int f, MATRIX3 *pmat);
VECTOR *VECTOR_opAssign(VECTOR *pv, qword q);
CRef RefOpMatrixTiltUpright__FiP4CRefP6CFrame(void *pbasic, CRef *aref);
}
/** @sly2-annot addr=0x1232D0
 * Purpose: Splice builtin "matrix-tilt-upright": 0x11B938 tilt upright
 * Info: proposed name RefOpMatrixTiltUpright (T1); TU bif
 * Confidence: high [T1 0.93] - evidence: static analysis; sources: r1_116918
 * See: docs/reference/functions/splice-scripting.md#bif-0x11f440-0x1258f0
 * @end-sly2-annot */
CRef RefOpMatrixTiltUpright__FiP4CRefP6CFrame(void *pbasic, CRef *aref)
{
    MATRIX3 mat;
    TiltMatUpright((XF *)aref[0].u.pv, 0, &mat);
    CRef ref(0x1B);
    MATRIX4 *pmat = (MATRIX4 *)ref.u.pv;
    VECTOR_opAssign(&pmat->pos, ((XF *)aref[0].u.pv)->pos);
    *(MATRIX3 *)pmat = mat;
    pmat->mat[2].w = 0.0f;
    pmat->mat[1].w = 0.0f;
    pmat->mat[0].w = 0.0f;
    pmat->pos.w = 1.0f;
    return ref;
}
