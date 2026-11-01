typedef char *va_list;
#define va_start(ap, last) (ap = ((char *)__builtin_next_arg(last) - (__builtin_args_info(2) >= 8 ? 0 : (8 - __builtin_args_info(2)) * 8)))
struct CRef {
    int m_tagk;
    int m_tag;
    CRef() { m_tagk = -1; }
    CRef(const CRef &);
    ~CRef() { Decref(); }
    CRef &operator=(const CRef &);
    void Decref();
};
struct FRAME {
    int a[4];
    FRAME();
    ~FRAME();
};
struct V { int c; void *pv; };
struct G;
struct H;
extern G D_003D4A60;
extern H D_003E0EA8;
void func_001D87C0(const char *p, va_list ap, V *pv);
CRef func_001D7ED8(H *, V *, G *);
static inline CRef Eval(V *pv, G *pg) { return func_001D7ED8(&D_003E0EA8, pv, pg); }
/** @sly2-annot addr=0x1D8A38
 * Purpose: CRef GetSaveVar(const char *name, ...): build key on stack frame and read from g_gs D_003D4A60
 * Info: proposed name RefGetSaveVarV (T1); TU save
 * Notes: runtime: save-var trie walk yields 4472 named vars
 * Confidence: high [T1 0.90] - evidence: runtime (save-state RAM); sources:
 *   r6_1D5150+verdicts_tu_r4r6+runtime
 * See: docs/engine/save-memcard.md#1-the-save-blob-g_gs-savecpp-0x1d7b88-0x1d93d8,
 *   docs/engine/save-memcard.md#5-key-functions,
 *   docs/reference/functions/save-memcard.md#save-0x1d7b88-0x1d93d8
 * @end-sly2-annot */
extern "C" CRef func_001D8A38(const char *fmt, ...)
{
    CRef ref;
    FRAME frame;
    V v;
    va_list ap;
    va_start(ap, fmt);
    func_001D87C0(fmt, ap, &v);
    ref = Eval(&v, &D_003D4A60);
    return ref;
}
