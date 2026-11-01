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
struct VTE { short d; short pad; void (*pfn)(void *, int, void *); };
struct LO { char pad[0xC]; VTE *pvt; };
extern "C" { CRef RefThunk_FireMissile(LO *p, int c, void *aref); }
/** @sly2-annot addr=0x104E80
 * Purpose: Splice option thunk: optid 508 on MISSILE (g_aeopid[405] thunkFn, returns Void, crefReq 2);
 *   unpacks 2 args (0x2030, Vector) -> Void via RefSetArgsFromSplice then calls virtual MISSILE slot 78 ->
 *   FireMissile (0x1B1E88)
 * Info: proposed name RefThunk_FireMissile (T1); TU crt0_splicewrap
 * Confidence: mid (draft) [T1 0.60] - evidence: static analysis; sources: r0_thunks+table-verified(g_aeopid
 *   D_002AC880)
 * See: docs/reference/functions/crt0-thunks.md#crt0_splicewrap-0x100000-0x1147d8
 * @end-sly2-annot */
CRef RefThunk_FireMissile(LO *p, int c, void *aref)
{
    int ak[2] = {0x2030, 2};
    union { qword q; } u;
    int n;
    CRef ref = func_00201360(2, aref, ak, &n, &u);
    ref.Decref();
    ref.InitTag(0x24);
    VTE *pvte = &p->pvt[78];
    pvte->pfn((char *)p + pvte->d, n, &u);
    return ref;
}
