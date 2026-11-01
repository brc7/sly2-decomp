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
extern "C" { void func_001D4B48(void *p, int n, void *pu, float g); CRef func_001104F0(void *p, int c, void *aref); }
/** @sly2-annot addr=0x1104F0
 * Purpose: Splice option thunk: optid 1215 on CID77 (g_aeopid[1419] thunkUser, returns Void, crefReq 3);
 *   unpacks 3 args (Oid, Float, Lm) -> Void via RefSetArgsFromSplice then calls AddRcCarWheel (0x1D4B48)
 * Info: proposed name RefThunk_AddRobottankWheel (T2); TU crt0_splicewrap
 * Notes: class identity from whole_game WE names/object strings: RefThunk_AddRcCarWheel ->
 *   RefThunk_AddRobottankWheel | T2 forced: thunk name is only as good as its native (0x1D4B48 T2)
 * Confidence: low (draft) [T2 0.55] - evidence: static analysis; sources: r0_thunks+table-verified(g_aeopid
 *   D_002AC880)+whole_game(class identity)
 * See: docs/reference/functions/crt0-thunks.md#crt0_splicewrap-0x100000-0x1147d8
 * @end-sly2-annot */
CRef func_001104F0(void *p, int c, void *aref)
{
    int ak[3] = {6, 1, 0xa};
    union { qword q; } u;
    int n;
    float g;
    CRef ref = func_00201360(3, aref, ak, &n, &g, &u);
    ref.Decref();
    ref.InitTag(0x24);
    func_001D4B48(p, n, &u, g);
    return ref;
}
