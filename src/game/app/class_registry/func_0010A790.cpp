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
extern "C" CRef RefSetArgsFromSplice__FiP4CRefP4OTYPe(int c, void *aref, int *ak, ...);
extern "C" { void func_00141968(void *p, qword, float, float); CRef func_0010A790(void *p, int c, void *aref); }
/** @sly2-annot addr=0x10A790
 * Purpose: Splice option thunk: optid 720 on STEPPLAYER (g_aeopid[730] thunkFn, returns Void, crefReq 3);
 *   unpacks 3 args (Vector, Float, Float) -> Void via RefSetArgsFromSplice then calls LaunchStepplayer
 *   (0x141968)
 * Info: proposed name RefThunk_LaunchStepplayer (T2); TU crt0_splicewrap
 * Notes: T2 forced: thunk name is only as good as its native (0x141968 T2)
 * Confidence: low (draft) [T2 0.55] - evidence: static analysis; sources: r0_thunks+table-verified(g_aeopid
 *   D_002AC880)
 * See: docs/reference/functions/crt0-thunks.md#crt0_splicewrap-0x100000-0x1147d8
 * @end-sly2-annot */
CRef func_0010A790(void *p, int c, void *aref)
{
    int ak[3] = {0x2, 0x1, 0x1};
    union { qword q; } v0;
    float v1;
    float v2;
    CRef ref = RefSetArgsFromSplice__FiP4CRefP4OTYPe(3, aref, ak, &v0, &v1, &v2);
    ref.Decref();
    ref.InitTag(0x24);
    func_00141968(p, v0.q, v1, v2);
    return ref;
}
