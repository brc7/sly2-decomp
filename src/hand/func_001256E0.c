typedef struct CRef { int m_tagk; int m_tag; } CRef;
void Decref__4CRef(CRef *pref);
CRef *__4CRefRC4CRef(CRef *pref, CRef *prefSrc);
void func_00129598(CRef *pref, int f);
int func_001DAEA8(int n);
/** @sly2-annot addr=0x1256E0
 * Purpose: Bool: save var powerUps.mppukfAcquired[n] via 0x1DAEA8
 * Info: proposed name RefOpIsPowerUpAcquired (T1); TU bif
 * Notes: splice builtin 138 power-up-acquired?~ -> power-up-acquired?~ (0.80): Body Bool(FPowerupAcquired(n))
 *   = save var powerUps.mppukfAcquired[n]. Scripts: (if (power-up-acquired?~ 27) ...)
 * Confidence: high [T1 0.80] - evidence: semantic review; sources: r1_116918+top50_round(splice builtin 138)
 * See: docs/reference/functions/splice-scripting.md#bif-0x11f440-0x1258f0
 * @end-sly2-annot */
CRef *func_001256E0(CRef *ret, void *pbasic, CRef *aref)
{
    CRef ref;
    ref.m_tagk = -1;
    func_00129598(&ref, func_001DAEA8(aref->m_tag) != 0);
    __4CRefRC4CRef(ret, &ref);
    Decref__4CRef(&ref);
    return ret;
}
