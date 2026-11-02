typedef int TI __attribute__((mode(TI)));
typedef struct CRef { int m_tagk; void *m_p; } CRef;
void Decref__4CRef(CRef *pref);
CRef *__4CRefRC4CRef(CRef *pref, CRef *prefSrc);
void SetTag__4CRef4TAGK(CRef *pref, int tagk);
TI ConvertAloVec(void *a, void *b, TI v);
/** @sly2-annot addr=0x125060
 * Purpose: Splice builtin "convert-object-vector": ConvertAloVec 0x1A6FC0
 * Info: proposed name RefOpConvertObjectVector (T1); TU bif
 * Notes: names_v2 B proposed RefOpHitTestObjectsFirst__FiP4CRefP6CFrame (0.923)
 * Confidence: high [T1 0.85] - evidence: static naming pass; sources: r1_116918+verdicts_tu_r1r2
 * See: docs/engine/splice-scripting.md#7-key-functions,
 *   docs/reference/functions/splice-scripting.md#bif-0x11f440-0x1258f0
 * @end-sly2-annot */
CRef *RefOpConvertObjectVector__FiP4CRefP6CFrame(CRef *ret, void *pbasic, CRef *aref)
{
    CRef ref;
    TI *pv;
    SetTag__4CRef4TAGK(&ref, 0x1A);
    pv = ref.m_p;
    *pv = ConvertAloVec(aref[0].m_p, aref[1].m_p, *(TI *)aref[2].m_p);
    __4CRefRC4CRef(ret, &ref);
    Decref__4CRef(&ref);
    return ret;
}
