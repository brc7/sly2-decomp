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
extern "C" { void *EmitSwNoise(void *p, int a, qword q, int b); CRef RefThunk_EmitSwNoise(void *p, int c, void *aref); }
/** @sly2-annot addr=0x10BFF8
 * Purpose: Splice option thunk: optid 850 on SW (g_aeopid[932] thunkFn, returns 0x2093, crefReq 3); unpacks 3
 *   args (0x1046, Vector, 0x2002) -> 0x2093 via RefSetArgsFromSplice then calls EmitSwNoise (0x156B98)
 * Info: proposed name RefThunk_EmitSwNoise (T1); TU crt0_splicewrap
 * Notes: v3: native 0x156B98 now T1 -> thunk T1
 * Confidence: mid (draft) [T1 0.55] - evidence: static analysis; sources: r0_thunks+table-verified(g_aeopid
 *   D_002AC880)
 * See: docs/reference/functions/crt0-thunks.md#crt0_splicewrap-0x100000-0x1147d8
 * @end-sly2-annot */
CRef RefThunk_EmitSwNoise(void *p, int c, void *aref)
{
    int ak[3] = {0x1046, 2, 0x2002};
    union { qword q; } u;
    int a, b;
    CRef ref = RefSetArgsFromSplice__FiP4CRefP4OTYPe(3, aref, ak, &a, &u, &b);
    ref = EmitSwNoise(p, a, u.q, b);
    return ref;
}
