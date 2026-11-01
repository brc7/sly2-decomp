typedef struct CRef { int m_tagk; int m_tag; } CRef;
void Decref__4CRef(CRef *pref);
CRef *__4CRefRC4CRef(CRef *pref, CRef *prefSrc);
void func_00129A10(CRef *pref, void *p);
extern char D_003DA0C0[];
/** @sly2-annot addr=0x125690
 * Purpose: CRef(BASIC*) of static object D_003DA0C0 (used by save TU 0x1D94B0)
 * Info: proposed name RefOpFindHintboxObject (T2); TU bif
 * Notes: v3: superseded RefOpFindGameObject | splice builtin 137 find-ticker-object~ -> find-hintbox-object~
 *   (0.55): Body CRef(BASIC*) of static D_003DA0C0. Lilly-derived code calls ClearIfSlotId(HINTBOX *phintbox,
 *   ...) with &D_003DA0C0 (func_00194A40.cpp:1518, idHintGrab), r7
 * Confidence: low (draft) [T2 0.55] - evidence: Sly 1 / Lilly reference; semantic review; sources:
 *   r1_116918+top50_round(splice builtin 137)
 * See: docs/reference/functions/splice-scripting.md#bif-0x11f440-0x1258f0
 * @end-sly2-annot */
CRef *func_00125690(CRef *ret)
{
    CRef ref;
    ref.m_tagk = -1;
    func_00129A10(&ref, D_003DA0C0);
    __4CRefRC4CRef(ret, &ref);
    Decref__4CRef(&ref);
    return ret;
}
