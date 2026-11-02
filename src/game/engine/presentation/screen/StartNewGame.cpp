extern "C" { void *func_001291A8(void *, const void *); void func_00129EC8(void *); }
class CRef {
public:
    int m_tagk;
    int m_tag;
    CRef(int tagk, int n) { m_tagk = tagk; m_tag = n; }
    CRef(const CRef &ref) { func_001291A8(this, &ref); }
    ~CRef() { func_00129EC8(this); }
};
struct G;
extern G D_003D4A60;
extern void *D_003E0FAC;
extern void *D_002E1E40;
extern void *D_002DD5BC;
extern void *D_002F7900;
extern "C" {
CRef RefSetGlobalSaveVar(void *, const CRef &, G *);
void BindStepplayerHpMp(void *, int);
void func_00165E38(G *);
}
/* The unused third parameter is deliberate: it is the same SetPv form as func_0013D328 / func_00143AF0,
   and the dummy makes gcc keep the %hi/%lo of D_003D4A60 in s0 across the whole function. */
static inline void SetPv(void *pv, const CRef &r, void *) { CRef arg(r); const CRef &ret = RefSetGlobalSaveVar(pv, arg, &D_003D4A60); }
/** @sly2-annot addr=0x1DDB08
 * Purpose: Clear the save flag at D_003E0FAC, bind hp/power of the three heroes, PlayGameFromGs(g_gs)
 * Info: proposed name StartNewGame (T1); TU screen
 * Confidence: mid (draft) [T1 0.55] - evidence: static analysis; sources: r9_bhv_screen
 * See: docs/reference/functions/hud-ui.md#screen-0x1d93d8-0x1de2a0
 * @end-sly2-annot */
extern "C" void StartNewGame(void)
{
    {
        CRef ref(0x16, 0);
        SetPv(D_003E0FAC, ref, &D_003D4A60);
    }
    if (D_002E1E40)
        BindStepplayerHpMp(D_002E1E40, 1);
    if (D_002DD5BC)
        BindStepplayerHpMp(D_002DD5BC, 1);
    if (D_002F7900)
        BindStepplayerHpMp(D_002F7900, 1);
    func_00165E38(&D_003D4A60);
}
