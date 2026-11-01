typedef struct CRef { int m_tagk; int m_tag; } CRef;
void Decref__4CRef(CRef *pref);
CRef *__4CRefRC4CRef(CRef *pref, CRef *prefSrc);
int func_00127D78(void *a, void *b, int c, CRef *d);
typedef struct BASIC { int pad[3]; void *p; } BASIC;
/** @sly2-annot addr=0x126438
 * Purpose: Evaluates a symbol: looks up its binding in the frame chain (0x127D78) and returns it
 * Info: proposed name RefEvalSymbol (T1); TU eval
 * Confidence: high [T1 0.88] - evidence: static analysis; sources: r1_116918
 * See: docs/engine/splice-scripting.md#1-layout-of-the-runtime,
 *   docs/engine/splice-scripting.md#6-source-files,
 *   docs/reference/functions/splice-scripting.md#eval-0x126438-0x127b18
 * @end-sly2-annot */
CRef *func_00126438(CRef *ret, BASIC *pbasic, void *aref)
{
    CRef ref;
    ref.m_tagk = -1;
    func_00127D78(aref, pbasic->p, 1, &ref);
    __4CRefRC4CRef(ret, &ref);
    Decref__4CRef(&ref);
    return ret;
}
