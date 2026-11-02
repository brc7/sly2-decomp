typedef struct CRef { int tagk; int tag; } CRef;
struct S { char p0[0x220]; int a220; int a224; };
void func_002013D8(CRef *pref, void *pv, int a, int b, int *pa, int *pb);
void func_00129EC8(CRef *pref);
void func_001D77D0(struct S *p, int n);
/** @sly2-annot addr=0x1D7780
 * Purpose: Fill +0x220/+0x224 from Splice list (0x2013D8) then reselect
 * Info: proposed name SetPoChoices (T1); TU tu_1D7238
 * Notes: verifier PLAUSIBLE: SetCharSelectChoices -> SetPoChoices / set allowed-selection list
 * Confidence: mid (draft) [T1 0.60] - evidence: static analysis; sources: r6_1D5150+verdicts_tu_r4r6
 * See: docs/reference/functions/player.md#tu_1d7238-0x1d7238-0x1d7a30
 * @end-sly2-annot */
void func_001D7780(struct S *p, void *pv) {
    CRef ref;
    func_002013D8(&ref, pv, 6, 6, &p->a220, &p->a224);
    func_00129EC8(&ref);
    func_001D77D0(p, 0);
}
