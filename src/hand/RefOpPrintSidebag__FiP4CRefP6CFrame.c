typedef struct CRef { int m_tagk; int m_tag; } CRef;
void Decref__4CRef(CRef *pref);
void InitTag__4CRef(CRef *pref, int tagk);
/** @sly2-annot addr=0x120330
 * Purpose: Splice builtin "print-sidebag": returns Void (local CRef then InitTag 0x24)
 * Info: proposed name RefOpPrintSidebag (T1); TU bif
 * Confidence: high [T1 0.93] - evidence: static analysis; sources: r1_116918
 * See: docs/reference/functions/splice-scripting.md#bif-0x11f440-0x1258f0
 * @end-sly2-annot */
CRef *RefOpPrintSidebag__FiP4CRefP6CFrame(CRef *ret)
{
    CRef ref;
    ref.m_tagk = -1;
    InitTag__4CRef(ret, 0x24);
    Decref__4CRef(&ref);
    return ret;
}
