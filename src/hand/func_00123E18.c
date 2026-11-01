typedef struct CRef { int m_tagk; union { int n; float g; void *pv; } u; } CRef;
void Decref__4CRef(CRef *pref);
void InitTag__4CRef(CRef *pref, int tagk);
void func_00129DA8(CRef *pref, CRef *prefSrc);
CRef *func_001291A8(CRef *pref, CRef *prefSrc);
extern float D_002DDED0;
/** @sly2-annot addr=0x123E18
 * Purpose: Splice builtin "defer-object-update": obj+0x30 = g_clock.t + dt
 * Info: proposed name RefOpDeferObjectUpdate (T1); TU bif
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r1_116918
 * See: docs/reference/functions/splice-scripting.md#bif-0x11f440-0x1258f0
 * @end-sly2-annot */
CRef *func_00123E18(CRef *ret, void *pbasic, CRef *aref)
{
    CRef ref;
    CRef refT;
    char *p;
    float g;
    InitTag__4CRef(&ref, 0x24);
    p = (char *)aref[0].u.pv;
    func_00129DA8(&refT, &aref[1]);
    g = refT.u.g;
    Decref__4CRef(&refT);
    *(float *)(p + 0x30) = D_002DDED0 + g;
    func_001291A8(ret, &ref);
    Decref__4CRef(&ref);
    return ret;
}
