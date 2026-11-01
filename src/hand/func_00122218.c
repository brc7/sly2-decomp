typedef struct CRef { int m_tagk; union { int n; float g; void *pv; } u; } CRef;
void Decref__4CRef(CRef *pref);
void InitTag__4CRef(CRef *pref, int tagk);
void func_00129DA8(CRef *pref, CRef *prefSrc);
CRef *func_001291A8(CRef *pref, CRef *prefSrc);
/** @sly2-annot addr=0x122218
 * Purpose: Splice builtin "smp": 3 floats -> tag 0x1E
 * Info: proposed name RefOpSmp (T1); TU bif
 * Notes: names_v2 B proposed RefUfo__FP4CRef4UFOK (0.980)
 * Confidence: high [T1 0.93] - evidence: static naming pass; sources: r1_116918+verify_side
 * See: docs/reference/functions/splice-scripting.md#bif-0x11f440-0x1258f0
 * @end-sly2-annot */
CRef *func_00122218(CRef *ret, void *pbasic, CRef *aref)
{
    CRef ref;
    int i;
    float *pg;
    InitTag__4CRef(&ref, 0x1E);
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
