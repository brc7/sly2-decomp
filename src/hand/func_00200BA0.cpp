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
extern "C" int func_001FFCF8(S *p, int n);
CRef func_001FFD98(S *p, int n) asm("func_001FFD98");
CRef func_00200BA0(S *p, int n) asm("func_00200BA0");
/** @sly2-annot addr=0x200BA0
 * Purpose: Option id <=0x4D0: RefGetPeopid(PeopidFind); else sidebag binding
 * Info: proposed name RefGetOption (T1); TU spliceobj
 * Confidence: high [T1 0.85] - evidence: static naming pass; sources:
 *   r7_1F8DD8+verdicts_A2+names_v2-signature(verified)
 * See: docs/formats/splice-scripts.md#3-names,
 *   docs/reference/functions/splice-scripting.md#spliceobj-0x1ffcf8-0x201620
 * @end-sly2-annot */
CRef func_00200BA0(S *p, int n)
{
    if (n <= 0x4D0) {
        return func_001FFD98(p, func_001FFCF8(p, n));
    }
    CRef ref;
    p->psidebag->FFindBinding(n, &ref);
    return ref;
}
