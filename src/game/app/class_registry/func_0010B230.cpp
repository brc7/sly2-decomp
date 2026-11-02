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
extern "C" { void func_00142310(void *p, float, float, void *, float, float, qword, qword); CRef func_0010B230(void *p, int c, void *aref); }
/** @sly2-annot addr=0x10B230
 * Purpose: Splice option thunk: optid 743 on STEPPLAYER (g_aeopid[760] thunkFn/thunkUser, returns Void,
 *   crefReq 7); unpacks 7 args (Float, Float, Lm, Float, Float, Vector, Vector) -> Void via
 *   RefSetArgsFromSplice then calls SetStepplayerThrowParams (0x142310)
 * Info: proposed name RefThunk_SetStepplayerThrowParams (T2); TU crt0_splicewrap
 * Notes: T2 forced: thunk name is only as good as its native (0x142310 T2)
 * Confidence: low (draft) [T2 0.55] - evidence: static analysis; sources: r0_thunks+table-verified(g_aeopid
 *   D_002AC880)
 * See: docs/reference/functions/crt0-thunks.md#crt0_splicewrap-0x100000-0x1147d8
 * @end-sly2-annot */
CRef func_0010B230(void *p, int c, void *aref)
{
    int ak[7] = {0x1, 0x1, 0xa, 0x1, 0x1, 0x2, 0x2};
    union { qword q; } v2;
    union { qword q; } v5;
    union { qword q; } v6;
    float v0;
    float v1;
    float v3;
    float v4;
    CRef ref = RefSetArgsFromSplice__FiP4CRefP4OTYPe(7, aref, ak, &v0, &v1, &v2, &v3, &v4, &v5, &v6);
    ref.Decref();
    ref.InitTag(0x24);
    func_00142310(p, v0, v1, &v2, v3, v4, v5.q, v6.q);
    return ref;
}
