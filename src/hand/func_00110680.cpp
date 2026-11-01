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
extern "C" { void func_001D4C48(void *p, int n, void *pu); CRef func_00110680(void *p, int c, void *aref); }
/** @sly2-annot addr=0x110680
 * Purpose: Splice option thunk: optid 1178 on CID77 (g_aeopid[1421] thunkUser, returns Void, crefReq 2);
 *   unpacks 2 args (Oid, Lm) -> Void via RefSetArgsFromSplice then calls AddRcCarAttachment (0x1D4C48)
 * Info: proposed name RefThunk_AddRobottankAttachment (T2); TU crt0_splicewrap
 * Notes: class identity from whole_game WE names/object strings: RefThunk_AddRcCarAttachment ->
 *   RefThunk_AddRobottankAttachment | T2 forced: thunk name is only as good as its native (0x1D4C48 T2)
 * Confidence: low (draft) [T2 0.55] - evidence: static analysis; sources: r0_thunks+table-verified(g_aeopid
 *   D_002AC880)+whole_game(class identity)
 * See: docs/reference/functions/crt0-thunks.md#crt0_splicewrap-0x100000-0x1147d8
 * @end-sly2-annot */
CRef func_00110680(void *p, int c, void *aref)
{
    int ak[2] = {6, 0xa};
    union { qword q; } u;
    int n;
    CRef ref = func_00201360(2, aref, ak, &n, &u);
    ref.Decref();
    ref.InitTag(0x24);
    func_001D4C48(p, n, &u);
    return ref;
}
