typedef struct CRef { int m_tagk; union { int n; float g; } u; } CRef;
void Decref__4CRef(CRef *pref);
CRef *__4CRefRC4CRef(CRef *pref, CRef *prefSrc);
void InitTag__4CRef(CRef *pref, int tagk);
void func_00129DA8(CRef *pref, CRef *prefSrc);
void func_00187E28(void *p, int n, float g);
extern char D_002DFC00[];
/** @sly2-annot addr=0x125248
 * Purpose: Splice builtin "start-rumble": TriggerJoyRumbleRumk-like 0x187E28
 * Info: proposed name RefOpStartRumble (T1); TU bif
 * Notes: names_v2 B proposed RefOpNearClipCenter__FiP4CRefP6CFrame (0.923)
 * Confidence: high [T1 0.85] - evidence: static naming pass; sources: r1_116918
 * See: docs/reference/functions/splice-scripting.md#bif-0x11f440-0x1258f0
 * @end-sly2-annot */
CRef *func_00125248(CRef *ret, void *pbasic, CRef *aref)
{
    void *p = D_002DFC00;
    int n = aref[0].u.n;
    CRef ref;
    func_00129DA8(&ref, &aref[1]);
    func_00187E28(p, n, ref.u.g);
    Decref__4CRef(&ref);
    InitTag__4CRef(&ref, 0x24);
    __4CRefRC4CRef(ret, &ref);
    Decref__4CRef(&ref);
    return ret;
}
