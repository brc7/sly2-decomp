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
extern "C" { void func_00181378(void *p, int, void *, void *); CRef func_0010FFE8(void *p, int c, void *aref); }
/** @sly2-annot addr=0x10FFE8
 * Purpose: Splice option thunk: optid 1183 on CID73 (g_aeopid[1381] thunkFn/thunkUser, returns Void, crefReq
 *   3); unpacks 3 args (0x105F, Lm, Lm) -> Void via RefSetArgsFromSplice then calls SetCid73EntryVectors2
 *   (0x181378)
 * Info: proposed name RefThunk_SetHackenemyEntryVectors2 (T2); TU crt0_splicewrap
 * Notes: class identity from whole_game WE names/object strings: RefThunk_SetCid73EntryVectors2 ->
 *   RefThunk_SetHackenemyEntryVectors2 | T2 forced: thunk name is only as good as its native (0x181378 T2)
 * Confidence: low (draft) [T2 0.55] - evidence: static analysis; sources: r0_thunks+table-verified(g_aeopid
 *   D_002AC880)+whole_game(class identity)
 * See: docs/reference/functions/crt0-thunks.md#crt0_splicewrap-0x100000-0x1147d8
 * @end-sly2-annot */
CRef func_0010FFE8(void *p, int c, void *aref)
{
    int ak[3] = {0x105f, 0xa, 0xa};
    union { qword q; } v1;
    union { qword q; } v2;
    int v0;
    CRef ref = func_00201360(3, aref, ak, &v0, &v1, &v2);
    ref.Decref();
    ref.InitTag(0x24);
    func_00181378(p, v0, &v1, &v2);
    return ref;
}
