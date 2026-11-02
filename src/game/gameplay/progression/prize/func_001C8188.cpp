extern "C" {
void *STACKSCOPE_ctor(void *p);
void STACKSCOPE_ctor_1AE4A0(void *p, int n);
void Decref__4CRef(void *p);
void *func_001291A8(void *p, const void *q);
void *FVaultOpened(void);
void func_001ABD98(void *p, int n);
}
class FRAME { public: FRAME() { STACKSCOPE_ctor(this); } ~FRAME() { STACKSCOPE_ctor_1AE4A0(this, 2); } };
struct CRef {
    int m_tagk; int m_tag;
    CRef() {}
    CRef(int f) { m_tagk = 0x17; m_tag = f; }
    CRef(const CRef &r) { func_001291A8(this, &r); }
    ~CRef() { Decref__4CRef(this); }
};
extern char D_003D4A60[];
extern char D_002E65E0[];

extern "C" CRef RefSetGlobalSaveVar(void *pv, const CRef &ref, void *p);
struct RAW { int m_tagk; int m_tag; };
extern "C" RAW *RefGetGlobalSaveVar(RAW *pr, void *pv, void *p);
static inline int NFromPv(void *pv,RAW &ref) { int n; RefGetGlobalSaveVar(&ref,pv,D_003D4A60);n=ref.m_tag;Decref__4CRef(&ref);return n; }
static inline void SetPv(void *pv,const CRef&r) { CRef arg(r); const CRef &ret=RefSetGlobalSaveVar(pv,arg,D_003D4A60); }
extern "C" void func_001C8188(int f);
/** @sly2-annot addr=0x1C8188
 * Purpose: set fVault and commit (0x1ABD98 memcard/save)
 * Info: proposed name SetVaultOpened (T2); TU prize
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r5_1AE130
 * See: docs/reference/functions/game-progression.md#prize-0x1c4e50-0x1c8cc0
 * @end-sly2-annot */
void func_001C8188(int f)
{
    FRAME frame; RAW ref;
    void *pv = FVaultOpened();
    if (!pv) return;
    if (f) {
        if (NFromPv(pv,ref) == 0)
            func_001ABD98(D_002E65E0, 0);
    }
    { CRef r(f!=0);SetPv(pv,r); }
}
