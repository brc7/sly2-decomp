extern "C" { void *func_001291A8(void *, const void *); void func_00129EC8(void *); }
class CRef {
public:
    int m_tagk;
    int m_tag;
    CRef(int tagk, int n) { m_tagk = tagk; m_tag = n; }
    CRef(const CRef &ref) { func_001291A8(this, &ref); }
    ~CRef() { func_00129EC8(this); }
};
struct FRAME {
    int a[4];
    FRAME();
    ~FRAME();
};
struct S { char pad[0x194]; void *f194; };
struct O;
extern char D_003D4A60[];
extern "C" {
void SetBootyState(void *, int);
O *func_0013D1A8(S *);
void func_0013CF38(S *, int);
CRef RefSetGlobalSaveVar(O *, const CRef &, void *);
}
static inline void SetPv(O *pv, const CRef &r, void *) { CRef arg(r); const CRef &ret = RefSetGlobalSaveVar(pv, arg, D_003D4A60); }
/** @sly2-annot addr=0x13D328
 * Purpose: Splice: reset current booty, save kind n, and spawn if n != -1
 * Info: proposed name SetBootyContainerKind (T2); TU booty
 * Confidence: low (draft) [T2 0.35] - evidence: static analysis; sources: r2b_rest
 * See: docs/reference/functions/game-progression.md#booty-0x13bdf0-0x13d558
 * @end-sly2-annot */
extern "C" void func_0013D328(S *p, int n)
{
    if (p->f194) SetBootyState(p->f194, 0);
    FRAME frame;
    {
        O *po = func_0013D1A8(p);
        CRef ref(0x15, n);
        SetPv(po, ref, D_003D4A60);
    }
    if (n != -1) func_0013CF38(p, 0);
}
