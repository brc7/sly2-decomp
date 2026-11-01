typedef struct CRef { int m_tagk; int m_tag; } CRef;
void Decref__4CRef(CRef *pref);
CRef *__4CRefRC4CRef(CRef *pref, CRef *prefSrc);
void InitTag__4CRef(CRef *pref, int tagk);
void func_001DAF38(int a, int b);
/** @sly2-annot addr=0x125738
 * Purpose: Sets powerUps.mppukfAcquired[n]=f via 0x1DAF38 (also fires event 0x3B on D_002DEB30 when set)
 * Info: proposed name RefOpSetPowerUpAcquired (T1); TU bif
 * Notes: splice builtin 139 set-power-up-acquired!~ -> set-power-up-acquired!~ (0.80): Body
 *   SetPowerupAcquired(n,f) (fires game event 0x3B when set). Scripts: (set-power-up-acquired!~ 31 #t) then
 *   SetStepplayerGadget:750 2 31
 * Confidence: high [T1 0.80] - evidence: semantic review; sources: r1_116918+top50_round(splice builtin 139)
 * See: docs/reference/functions/splice-scripting.md#bif-0x11f440-0x1258f0
 * @end-sly2-annot */
CRef *func_00125738(CRef *ret, void *pbasic, CRef *aref)
{
    CRef ref;
    InitTag__4CRef(&ref, 0x24);
    func_001DAF38(aref[0].m_tag, aref[1].m_tag);
    __4CRefRC4CRef(ret, &ref);
    Decref__4CRef(&ref);
    return ret;
}
