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
void BuildSaveKeyV(const char *p, va_list ap, V *pv);
CRef RefSetSaveVar(H *, V *, const CRef &, G *);
static inline CRef Eval(V *pv, const CRef &r, G *pg) { return RefSetSaveVar(&D_003E0EA8, pv, r, pg); }
/** @sly2-annot addr=0x1D8B20
 * Purpose: CRef SetSaveVar(const CRef &val, const char *name, ...) into g_gs
 * Info: proposed name RefSetSaveVarV (T1); TU save
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r6_1D5150+verdicts_tu_r4r6
 * See: docs/engine/save-memcard.md#1-the-save-blob-g_gs-savecpp-0x1d7b88-0x1d93d8,
 *   docs/engine/save-memcard.md#5-key-functions,
 *   docs/reference/functions/save-memcard.md#save-0x1d7b88-0x1d93d8
 * @end-sly2-annot */
extern "C" CRef RefSetSaveVarV(const CRef &refArg, const char *fmt, ...)
{
    CRef ref;
    FRAME frame;
    V v;
    va_list ap;
    va_start(ap, fmt);
    BuildSaveKeyV(fmt, ap, &v);
    ref = Eval(&v, refArg, &D_003D4A60);
    return ref;
}
