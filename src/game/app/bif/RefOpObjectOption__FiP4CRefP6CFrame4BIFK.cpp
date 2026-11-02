struct CRef {
    int m_tagk; union { int n; float g; void *pv; } u;
    CRef() { m_tagk = -1; }
    CRef(int tagk) { InitTag(tagk); }
    CRef(const CRef &r);
    void Decref();
    void InitTag(int tagk);
    ~CRef() { Decref(); }
    CRef &operator=(const CRef &r);
    CRef &operator=(float g);
};
extern "C" {
CRef RefAddOption__FP5BASICiP4CRef(void *a, void *b, CRef *pref);
CRef RefEnsureOption__FP5BASICiP4CRef(void *a, void *b, CRef *pref);
CRef RefSetOption__FP5BASICiP4CRef(void *a, void *b, CRef *pref);
CRef RefGetOption__FP5BASICi(void *a, void *b);
CRef RefOpObjectOption__FiP4CRefP6CFrame4BIFK(void *pbasic, CRef *aref, void *pv, int op);
}
/** @sly2-annot addr=0x123EB0
 * Purpose: Shared body of addo/ensureo/seto!/geto: switch on BIFK 0x6C..0x6F ->
 *   RefAddOption/RefEnsureOption/RefSetOption/RefGetOption
 * Info: proposed name RefOpObjectOption (T1); TU bif
 * Confidence: high [T1 0.95] - evidence: static analysis; sources: r1_116918+verdicts_B
 * See: docs/engine/splice-scripting.md#7-key-functions, docs/formats/splice-scripts.md#3-names,
 *   docs/reference/functions/splice-scripting.md#bif-0x11f440-0x1258f0
 * @end-sly2-annot */
CRef RefOpObjectOption__FiP4CRefP6CFrame4BIFK(void *pbasic, CRef *aref, void *pv, int op)
{
    CRef ref;
    void *a = aref[0].u.pv;
    void *b = aref[1].u.pv;
    switch (op) {
    case 0x6C:
        ref = RefAddOption__FP5BASICiP4CRef(a, b, &aref[2]);
        break;
    case 0x6D:
        ref = RefEnsureOption__FP5BASICiP4CRef(a, b, &aref[2]);
        break;
    case 0x6E:
        ref = RefSetOption__FP5BASICiP4CRef(a, b, &aref[2]);
        break;
    case 0x6F:
        ref = RefGetOption__FP5BASICi(a, b);
        break;
    }
    return ref;
}
