struct P { char pad[0x13D0]; int n13D0; char pad1[0x1430 - 0x13D4]; void *p1430; char pad2[0x1910 - 0x1434]; int a1910[1]; };
int func_00144388(struct P *p, void *pv);
int func_001A6AF8(void *pv, void *p1);
int func_00134310(int n, int k);
int func_00220498(int *pn, void *pv, int n);
/** @sly2-annot addr=0x1B8450
 * Purpose: MURRAY vt61(pso): base flags 0x144388; adds bit 3 for an object that is (part of) the recently
 *   released one x1430, or defers to JTTHROW filter in state 15
 * Info: proposed name MURRAY_FIgnoreIntersection (T1); class MURRAY slot61; TU murray
 * Notes: superseded: MURRAY_GrfIgnoreIntersection (verdicts_followups) | verifier better_name (CORRECT row):
 *   MURRAY vtable slot 61 (same slot as STEPPLAYER_FIgnoreIntersection, which it chains). If pv is under the
 *   recently released object (+0x1430) result = NMaxIgnore(
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r5b_rest+verdicts_followups
 * See: docs/engine/player.md#11-key-functions, docs/reference/functions/player.md#murray-0x1b4728-0x1b9f80
 * @end-sly2-annot */
int func_001B8450(struct P *p, void *pv)
{
    int n = func_00144388(p, pv);
    if (p->p1430 && func_001A6AF8(pv, p->p1430))
        n = func_00134310(n, 3);
    else if (p->n13D0 == 15)
        n = func_00220498(p->a1910, pv, n);
    return n;
}
