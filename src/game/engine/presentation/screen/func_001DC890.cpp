struct CRef {
    int m_tagk;
    int m_tag;
    CRef() { m_tagk = -1; }
    CRef(const CRef &);
    ~CRef() { Decref(); }
    CRef &operator=(const CRef &);
    void Decref();
};
struct ENT { int n; int a; };
struct TBL {
    char pad[0x4C];
    ENT ae[0x40];
    int i;
    int first;
};
struct REC { char *name; int ids; int pad; };
struct G;
extern G D_003DD490;
extern TBL D_003DA150;
extern REC D_002C3288[];
extern char D_002A79A0[];
extern char D_002A79D0[];
extern char D_002A79D8[];
extern char D_002A7A30[];
extern "C" int func_0021ECD8(char *, const char *, ...);
extern "C" char *func_0021ED58(char *, const char *);
extern "C" CRef func_001D8A38(const char *, ...);
char *PchzFindSwLangstr__FP2SWi(G *, int);
/** @sly2-annot addr=0x1DC890
 * Purpose: Text callback rows 0x448-0x44D: loot name of row i, "%s (%d)" when owned more than once
 * Info: proposed name BuildThiefnetLootNameText (T1); TU screen
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: r9_bhv_screen
 * See: docs/reference/functions/hud-ui.md#screen-0x1d93d8-0x1de2a0
 * @end-sly2-annot */
extern "C" void func_001DC890(char *pch, int k)
{
    int ids = -1;
    int c = 0;
    unsigned int i = D_003DA150.ae[D_003DA150.first + k].n;
    if (i < 0x40) {
        CRef ref;
        char ach[0x20];
        func_0021ECD8(ach, D_002A79A0, i);
        ref = func_001D8A38(D_002A79D0, D_002A79D8, ach, 0);
        c = ref.m_tag;
        ids = D_002C3288[i].ids;
    }
    if (ids != -1) {
        func_0021ED58(pch, PchzFindSwLangstr__FP2SWi(&D_003DD490, ids));
        if (c >= 2)
            func_0021ECD8(pch, D_002A7A30, pch, c);
    } else {
        *pch = 0;
    }
}
