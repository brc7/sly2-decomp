typedef struct CRef { int m_tagk; union { int n; float g; void *pv; } u; } CRef;
void Decref__4CRef(CRef *pref);
void InitTag__4CRef(CRef *pref, int tagk);
void func_00129DA8(CRef *pref, CRef *prefSrc);
CRef *func_001291A8(CRef *pref, CRef *prefSrc);
/** @sly2-annot addr=0x1220B8
 * Purpose: Splice builtin "clq": 3 floats -> tag 0x1C
 * Info: proposed name RefOpClq (T1); TU bif
 * Confidence: high [T1 0.93] - evidence: static analysis; sources: r1_116918+verdicts_B+verify_side
 * See: docs/engine/splice-scripting.md#7-key-functions,
 *   docs/reference/functions/splice-scripting.md#bif-0x11f440-0x1258f0
 * @end-sly2-annot */
CRef *func_001220B8(CRef *ret, void *pbasic, CRef *aref)
{
    CRef ref;
    int i;
    float *pg;
    InitTag__4CRef(&ref, 0x1C);
    pg = (float *)ref.u.pv;
    for (i = 0; i < 3; i++) {
        CRef tmp;
        func_00129DA8(&tmp, &aref[i]);
        pg[i] = tmp.u.g;
        Decref__4CRef(&tmp);
    }
    func_001291A8(ret, &ref);
    Decref__4CRef(&ref);
    return ret;
}
