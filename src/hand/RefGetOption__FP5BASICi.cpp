class CRef;
extern "C" void func_00129EC8(CRef *pref);
class CRef {
public:
    int m_tagk;
    int m_tag;
    CRef() { m_tagk = -1; }
    CRef(const CRef &ref) asm("func_001291A8");
    ~CRef() { func_00129EC8(this); }
};
class CSidebag { public: int FFindBinding(int sym, CRef *pref); };
struct S { int a; CSidebag *psidebag; };
extern "C" int PeopidFind(S *p, int n);
CRef RefGetPeopid(S *p, int n) asm("RefGetPeopid");
CRef RefGetOption__FP5BASICi(S *p, int n) asm("RefGetOption__FP5BASICi");
/** @sly2-annot addr=0x200BA0
 * Purpose: Option id <=0x4D0: RefGetPeopid(PeopidFind); else sidebag binding
 * Info: proposed name RefGetOption (T1); TU spliceobj
 * Confidence: high [T1 0.85] - evidence: static naming pass; sources:
 *   r7_1F8DD8+verdicts_A2+names_v2-signature(verified)
 * See: docs/formats/splice-scripts.md#3-names,
 *   docs/reference/functions/splice-scripting.md#spliceobj-0x1ffcf8-0x201620
 * @end-sly2-annot */
CRef RefGetOption__FP5BASICi(S *p, int n)
{
    if (n <= 0x4D0) {
        return RefGetPeopid(p, PeopidFind(p, n));
    }
    CRef ref;
    p->psidebag->FFindBinding(n, &ref);
    return ref;
}
