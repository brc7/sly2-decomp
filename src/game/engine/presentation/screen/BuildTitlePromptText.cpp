struct CRef {
    int m_tagk;
    union { int m_tag; float m_g; };
    CRef() { m_tagk = -1; }
    CRef(const CRef &);
    ~CRef() { Decref(); }
    void Decref();
};
struct G { char pad[0x3B1C]; void *x3B1C; };
struct H { char pad[0x100C]; int x100C; int x1010; };
struct I { char pad[0xFD54]; void *xFD54; };
extern G D_003DD490;
extern H D_002DFC00;
extern I D_002E65E0;
extern char D_002A7A70[];
extern "C" char *func_0021ED58(char *, const char *);
extern "C" char *func_0021EDA0(char *, const char *);
extern "C" int CLanguagesRegion(void);
char *PchzFindSwLangstr__FP2SWi(G *, int);
CRef RefGetGlobalSaveVar(void *, void *);
/** @sly2-annot addr=0x1DDBF0
 * Purpose: Text callback 0x5D3: title prompt (continue vs new game, options, language) from string ids
 *   5/6/8/0x56
 * Info: proposed name BuildTitlePromptText (T1); TU screen
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r9_bhv_screen+verdicts_followups
 * See: docs/engine/hud-ui.md#5-key-functions, docs/reference/functions/hud-ui.md#screen-0x1d93d8-0x1de2a0
 * @end-sly2-annot */
extern "C" void BuildTitlePromptText(char *pch)
{
    float g;
    void *pv;
    if (D_002DFC00.x100C == 1 && D_002DFC00.x1010 == 0) {
        func_0021ED58(pch, PchzFindSwLangstr__FP2SWi(&D_003DD490, 9));
        return;
    }
    g = 0.0f;
    pv = D_002E65E0.xFD54;
    if (pv) {
        CRef ref = RefGetGlobalSaveVar(D_003DD490.x3B1C, pv);
        g = ref.m_g;
    }
    {
        int ids = g != 0.0f ? 6 : 5;
        func_0021ED58(pch, PchzFindSwLangstr__FP2SWi(&D_003DD490, ids));
    }
    func_0021EDA0(pch, D_002A7A70);
    func_0021EDA0(pch, PchzFindSwLangstr__FP2SWi(&D_003DD490, 8));
    if (CLanguagesRegion() >= 2) {
        func_0021EDA0(pch, D_002A7A70);
        func_0021EDA0(pch, PchzFindSwLangstr__FP2SWi(&D_003DD490, 0x56));
    }
}
