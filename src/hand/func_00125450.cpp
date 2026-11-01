struct CRef {
    int m_tagk; union { int n; float g; void *pv; } u;
    CRef() { m_tagk = -1; }
    CRef(const CRef &r);
    void Decref();
    void InitTag(int tagk);
    ~CRef() { Decref(); }
    CRef &operator=(const CRef &r);
};
struct OBJ { int a[4]; OBJ(); ~OBJ(); };
struct X { int a[4]; };
extern char D_003E0EA8[];
extern char D_003D4A60[];
extern "C" {
void func_001252D0(void *pv, X *px);
CRef func_001D80C8(void *a, X *px, CRef *pref, void *b);
CRef func_00125450(void *pbasic, CRef *aref);
}
/** @sly2-annot addr=0x125450
 * Purpose: Path list + value -> RefSetSaveVar 0x1D80C8
 * Info: proposed name RefOpSetSaveVar (T1); TU bif
 * Notes: splice builtin 132 set-save-var!~ -> set-save-var!~ (0.80): Body pair of 131 (RefSetSaveVar). Unused
 *   by retail scripts
 * Confidence: high [T1 0.80] - evidence: semantic review; sources: r1_116918+top50_round(splice builtin 132)
 * See: docs/reference/functions/splice-scripting.md#bif-0x11f440-0x1258f0
 * @end-sly2-annot */
CRef func_00125450(void *pbasic, CRef *aref)
{
    CRef ref;
    OBJ obj;
    X x;
    func_001252D0(aref[0].u.pv, &x);
    ref = func_001D80C8(D_003E0EA8, &x, &aref[1], D_003D4A60);
    return ref;
}
