typedef struct CRef { int m_tagk; int m_tag; } CRef;
void Decref__4CRef(CRef *pref);
CRef *__4CRefRC4CRef(CRef *pref, CRef *prefSrc);
void InitTag__4CRef(CRef *pref, int tagk);
void func_00129658(CRef *pref, CRef *prefSrc);
CRef *func_00127DB8(void *a, void *b, int c);
/** @sly2-annot addr=0x127CF0
 * Purpose: PrefFindBinding(sym,1) and assign; returns Void
 * Info: proposed name CFrame_RefSetBinding (T1); class CFrame; TU frame
 * Confidence: high [T1 0.88] - evidence: static naming pass; sources:
 *   r1_116918+verdicts_A1+names_v2-signature(verified)
 * See: docs/engine/splice-scripting.md#7-key-functions,
 *   docs/reference/functions/splice-scripting.md#eval-0x126438-0x127b18,
 *   docs/reference/functions/splice-scripting.md#frame-0x127b18-0x128138
 * @end-sly2-annot */
CRef *RefSetBinding__6CFrameUiP4CRef(CRef *ret, void *a, void *b, CRef *pref)
{
    CRef ref;
    CRef *p;
    ref.m_tagk = -1;
    p = func_00127DB8(a, b, 1);
    if (p)
        func_00129658(p, pref);
    Decref__4CRef(&ref);
    InitTag__4CRef(&ref, 0x24);
    __4CRefRC4CRef(ret, &ref);
    Decref__4CRef(&ref);
    return ret;
}
