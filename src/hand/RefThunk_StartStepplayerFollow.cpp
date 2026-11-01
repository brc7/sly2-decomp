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
    CRef &operator=(void *pv);
    CRef &operator=(qword q);
    CRef &operator=(bool f);
    CRef &operator=(int n);
    CRef &operator=(float g);
};
extern "C" CRef func_00201360(int c, void *aref, int *ak, ...);
extern "C" { void StartStepplayerFollow(void *p, int n, qword q); CRef RefThunk_StartStepplayerFollow(void *p, int c, void *aref); }
/** @sly2-annot addr=0x10A940
 * Purpose: Splice option thunk: optid 446 on STEPPLAYER (g_aeopid[733] thunkFn, returns Void, crefReq 2);
 *   unpacks 2 args (0x2002, Vector) -> Void via RefSetArgsFromSplice then calls StartStepplayerBhvC2
 *   (0x142540)
 * Info: proposed name RefThunk_StartStepplayerFollow (T1); TU crt0_splicewrap
 * Notes: v3 thunk renamed with its native 0x142540: StartStepplayerBhvC2 -> StartStepplayerFollow | v3:
 *   native 0x142540 now T1 -> thunk T1
 * Confidence: mid (draft) [T1 0.55] - evidence: static analysis; sources: r0_thunks+table-verified(g_aeopid
 *   D_002AC880)
 * See: docs/reference/functions/crt0-thunks.md#crt0_splicewrap-0x100000-0x1147d8
 * @end-sly2-annot */
CRef RefThunk_StartStepplayerFollow(void *p, int c, void *aref)
{
    int ak[2] = {0x2002, 2};
    union { qword q; } u;
    int n;
    CRef ref = func_00201360(2, aref, ak, &n, &u);
    ref.Decref();
    ref.InitTag(0x24);
    StartStepplayerFollow(p, n, u.q);
    return ref;
}
