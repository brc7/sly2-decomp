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
CRef func_001D7ED8(void *a, X *px, void *b);
CRef func_00125398(void *pbasic, CRef *aref);
}
/** @sly2-annot addr=0x125398
 * Purpose: Converts path list to U16 array (0x1252D0) and returns RefGetSaveVar(D_003E0EA8,..)
 * Info: proposed name RefOpGetSaveVar (T1); TU bif
 * Notes: names_v2 B proposed RefOpStartRumble__FiP4CRefP6CFrame (0.923) | splice_scripts format doc cites
 *   this function by this name (CONFIRMED(code) in the doc) | splice builtin 131 get-save-var~ ->
 *   get-save-var~ (0.85): Body converts the quoted path list to a key array (FillAsFromPair) and returns
 *   RefGetSaveVar(D_003E0EA8, key, D_003D4A60). Scripts: (get-save-var~ '(oidPoCurren
 * Confidence: high [T1 0.85] - evidence: semantic review; static naming pass; sources:
 *   r1_116918+format-doc:splice_scripts+top50_round(splice builtin 131)
 * See: docs/formats/splice-scripts.md#3-names,
 *   docs/reference/functions/splice-scripting.md#bif-0x11f440-0x1258f0
 * @end-sly2-annot */
CRef func_00125398(void *pbasic, CRef *aref)
{
    CRef ref;
    OBJ obj;
    X x;
    func_001252D0(aref[0].u.pv, &x);
    ref = func_001D7ED8(D_003E0EA8, &x, D_003D4A60);
    return ref;
}
