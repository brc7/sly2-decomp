typedef int TI __attribute__((mode(TI)));
typedef struct CRef { int m_tagk; int m_tag; } CRef;
void Decref__4CRef(CRef *pref);
CRef *__4CRefRC4CRef(CRef *pref, CRef *prefSrc);
void InitTag__4CRef(CRef *pref, int tagk);
void CRef_ctor(CRef *pref, TI vec);
TI ConvertAloPos(int a, int b, TI v);
/** @sly2-annot addr=0x125000
 * Purpose: Splice builtin "convert-object-position": ConvertAloPos 0x1A6F00
 * Info: proposed name RefOpConvertObjectPosition (T1); TU bif
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r1_116918
 * See: docs/reference/functions/splice-scripting.md#bif-0x11f440-0x1258f0
 * @end-sly2-annot */
CRef *RefOpConvertObjectPosition__FiP4CRefP6CFrame(CRef *ret, void *pbasic, CRef *aref)
{
    CRef ref;
    CRef_ctor(&ref, ConvertAloPos(aref[0].m_tag, aref[1].m_tag, *(TI *)aref[2].m_tag));
    __4CRefRC4CRef(ret, &ref);
    Decref__4CRef(&ref);
    return ret;
}
