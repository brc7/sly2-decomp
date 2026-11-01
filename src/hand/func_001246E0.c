typedef struct CRef { int m_tagk; int m_tag; } CRef;
void Decref__4CRef(CRef *pref);
CRef *__4CRefRC4CRef(CRef *pref, CRef *prefSrc);
void func_00129A10(CRef *pref, void *p);
extern char D_002DDF40[];
/** @sly2-annot addr=0x1246E0
 * Purpose: Splice builtin "find-camera-object": CRef(BASIC*) of &D_002DDF40 (g_cm)
 * Info: proposed name RefOpFindCameraObject (T1); TU bif
 * Confidence: high [T1 0.93] - evidence: static analysis; sources: r1_116918
 * See: docs/reference/functions/splice-scripting.md#bif-0x11f440-0x1258f0
 * @end-sly2-annot */
CRef *func_001246E0(CRef *ret)
{
    CRef ref;
    ref.m_tagk = -1;
    func_00129A10(&ref, D_002DDF40);
    __4CRefRC4CRef(ret, &ref);
    Decref__4CRef(&ref);
    return ret;
}
