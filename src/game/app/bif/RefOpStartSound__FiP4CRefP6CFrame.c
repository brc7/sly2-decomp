typedef struct CRef { int m_tagk; int m_tag; } CRef;
void Decref__4CRef(CRef *pref);
CRef *__4CRefRC4CRef(CRef *pref, CRef *prefSrc);
void InitTag__4CRef(CRef *pref, int tagk);
void *PsndStartSound(int n);
void SetSndSourceLo(void *p, int n);
void CRef_ctor_BASIC(CRef *pref, void *p);
/** @sly2-annot addr=0x1251E8
 * Purpose: Splice builtin "start-sound": 0x1FBB90/0x1FB2F8 sound start, returns CRef(BASIC*) of sound
 * Info: proposed name RefOpStartSound (T1); TU bif
 * Notes: names_v2 C proposed RefOpConvertObjectMatrix__FiP4CRefP6CFrame (0.200)
 * Confidence: high [T1 0.85] - evidence: static naming pass; sources: r1_116918
 * See: docs/reference/functions/splice-scripting.md#bif-0x11f440-0x1258f0
 * @end-sly2-annot */
CRef *RefOpStartSound__FiP4CRefP6CFrame(CRef *ret, void *pbasic, CRef *aref)
{
    int n = aref[1].m_tag;
    void *p = PsndStartSound(aref[0].m_tag);
    SetSndSourceLo(p, n);
    CRef_ctor_BASIC(ret, p);
    return ret;
}
