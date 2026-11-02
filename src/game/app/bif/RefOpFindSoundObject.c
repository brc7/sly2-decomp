typedef struct CRef { int m_tagk; int m_tag; } CRef;
void Decref__4CRef(CRef *pref);
CRef *__4CRefRC4CRef(CRef *pref, CRef *prefSrc);
void CRef_SetBasic(CRef *pref, void *p);
extern char D_003DD040[];
/** @sly2-annot addr=0x124690
 * Purpose: CRef(BASIC*) of static object D_003DD040 (the object LoadSwFromBrx passes to 0x1FD0E8
 *   sound-section loader)
 * Info: proposed name RefOpFindSoundObject (T1); TU bif
 * Notes: names_v2 C proposed RefOpFindWorldObject__FiP4CRefP6CFrame (0.200) | splice builtin 118
 *   find-sound-object~ -> find-sound-object~ (0.65): Body CRef(BASIC*) of g_sndmgr D_003DD040 (sound.md:
 *   built by sound.cpp static init). Methods called on it: StartDialog:264, FDialogPlaying:266,
 *   SetSndmgrChannel
 * Confidence: mid (draft) [T1 0.65] - evidence: semantic review; static naming pass; sources:
 *   r1_116918+top50_round(splice builtin 118)
 * See: docs/reference/functions/splice-scripting.md#bif-0x11f440-0x1258f0
 * @end-sly2-annot */
CRef *RefOpFindSoundObject(CRef *ret)
{
    CRef ref;
    ref.m_tagk = -1;
    CRef_SetBasic(&ref, D_003DD040);
    __4CRefRC4CRef(ret, &ref);
    Decref__4CRef(&ref);
    return ret;
}
